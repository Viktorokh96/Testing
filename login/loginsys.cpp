#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unistd.h>
using namespace std;

#define HKEY 2641761653
#define NAME_SIZE 41
#define DB_PATH "test.txt"

// ???
#define RESULT_ERROR 0 
#define RESULT_OK 1

typedef unsigned long int hashData;
class Hash {
protected:
	const hashData _hashKey; // ключ
	const hashData _hashValue; // зашифрованное поле

	virtual hashData hash( string item ) // метод шифрования
	{
		hashData hdret = 0;
		const char *txtch = item.c_str();
		unsigned char acc = (char) _hashKey;

		for(; *txtch; txtch++) {
			const char *tmpch = txtch;
			for (; *tmpch; tmpch++)
				acc ^= *tmpch;
			hdret ^= (_hashKey * acc);
			acc = (char) hdret;
		}
		return hdret;
	}

	virtual hashData readHashKey(istream &stream) { // метод чтения ключа
		hashData retVal; 
		stream.read(reinterpret_cast<char*>(&retVal),sizeof(hashData)); // чтение ключа из бинарного файла, 
		//reinterpret_cast<char*>(&retVal) приводит двоичный код к типу char* и записывает полученную строку в retVal

		return retVal;
	}
	virtual hashData readHashValue(istream &stream) { // метод чтения зашифрованного поля
		hashData retVal;
		stream.read(reinterpret_cast<char*>(&retVal),sizeof(hashData));

		return retVal;
	}
public:
	Hash(string passw) : _hashKey( HKEY ), _hashValue( hash(passw) )
	{
	}
	// Перегрузка конструктора Hash
	Hash ( istream &stream ) : _hashKey( readHashKey(stream) ), _hashValue( readHashValue(stream) ) 
	{
	}
	// Перегрузка конструктора Hash	
	Hash(Hash &hashParam) : _hashKey( hashParam._hashKey ), _hashValue( hashParam._hashValue )
	{
	}
	// Деструктор
	virtual ~Hash() {}
	
	// перегрузка операторов
	friend ostream &operator<< (ostream &stream, Hash hashObj); // запись в поток
};

ostream &operator<< (ostream &stream, Hash hashObj) 
{
	stream.write(reinterpret_cast<const char*>(&hashObj._hashKey),sizeof(hashObj._hashKey)); // запись ключа
	stream.write(reinterpret_cast<const char*>(&hashObj._hashValue),sizeof(hashObj._hashValue)); // запись шифрованного поля

	return stream;
}

class Password : Hash
{
public:
	// Вызов конструкторов предка
	Password(string passw) : Hash(passw) 
	{}
	Password(istream &stream) : Hash(stream)
	{}
	Password(Hash &hashParam) : Hash(hashParam)
	{}
	// Деструктор
	virtual ~Password() {}

	hashData hashPassword ( string passw ) // шифровка пароля
	{
		return hash(passw);
	}
	bool compare( string passw ) // сравнение зашифрованных полей
	{
		return (_hashValue == hash(passw));
	}
	friend ostream &operator<< (ostream &stream, Password passObj); // запись в поток
};

ostream &operator<< (ostream &stream, Password passObj) 
{
	stream.write(reinterpret_cast<const char*>(&passObj._hashKey),sizeof(passObj._hashKey)); // запись ключа
	stream.write(reinterpret_cast<const char*>(&passObj._hashValue),sizeof(passObj._hashValue)); // запись шифрованного поля

	return stream;
}

class User
{
	char _name[NAME_SIZE]; // сделать динамический!
	Password *_password;
public:
	// Конструктор пользователя
	User(){ _password = NULL; _name[0] = '\0'; }
	// Ещё один конструктор
	User(const char name[NAME_SIZE], string passw)
	{ 
		 setName(name);
		 setPassword(passw);
	}
	// Конструктор копий пользователя
	User(const User &user) { 
		strncpy(_name, user._name, NAME_SIZE); // сделать динамический!
	       	_password = new Password(*user._password); 
	}	
	// Деструктор пользователя
	~User(){ if(_password) delete _password; }
	// Установка имени пользователя
	void setName(const char name[NAME_SIZE]) 
	{ 
		strncpy(_name, name, NAME_SIZE); // сделать динамический!
	} 
	// Получение имени пользователя
	string getName()
	{ 
		string retName = _name;
		return retName; 
	}
	// Установка нового пароля для пользователя
	void setPassword(string passw) {
		if(_password) delete _password; // если пароль уже был установлен, то нужно заменить его на новый
	 	_password = new Password(passw); 
	}
	// Сравнение имеющегося пароля с введённым
	bool comparePassword(string passw) { 
		if(_password) return _password->compare(passw); 
		else return false;
	}
		
	friend ostream &operator<< (ostream &stream, User user); // запись в поток
	friend istream &operator>> (istream &stream, User &user); // чтение из потока
};

istream &operator>> (istream &stream, User &user)
{
	if(user._password)
		delete user._password; 

	stream.read(reinterpret_cast<char*>(user._name),sizeof(user._name));
	user._password = new Password(stream);

	return stream;
}

ostream &operator<< (ostream &stream, User user)
{
	if( !user._password ) return stream;	// логин без пароля бесполезен

	stream.write(reinterpret_cast<char*>(&user._name),sizeof(user._name));
	stream << *user._password;

	return stream;
}


/* Поток ввода-вывода для класса User
 * Возлагает на себя обязанности по корректной
 * записи объектов класса User в файл, для 
 * последующего корректного чтения.
*/
class UserStream
{
	User _userinfo;
public:
	vector <User> read(char *path)
	{
		vector <User> userList;
		User user;

		userList.clear();
		
		ifstream file(path, ios_base::binary);
		if(!file) { 
			cerr << "Не удаётся открыть файл!\n";
			return userList;	// В случае ошибки вернётся пустой список пользователей
	       	}

		while(!file.eof())
		{
			file >> user;
			userList.push_back(user);
		}
		file.close();
		return userList;
	}

	int write(char *path, vector<User> userList) // вернёт 0 в случае ошибки
	{
		ofstream file(path, ios_base::binary);	// откроет бинарный файл и допишет в конец

		vector<User>::iterator iter = userList.begin();
		for(; iter != userList.end(); ++iter)
			file << *iter;

		file.close();

		return RESULT_OK;
	}
};

/*
 * Класс СУБД
 * При реализации этого класса расчёт вёлся на создание специализированной СУБД
 * по сути, хранящей в себе реализацию базы данных пользователей системы.
 * Предполагается, что схема данных такой БД жёстко определена ещё во время
 * компиляции программы ( это сделано с целью упрощения ).
 * В обязанности класса входят:
 * - Инкапсуляция реализации устройства и данных БД
 * - Интерфейс управления данными для стороннего пользователя, который включает:
 *   	% loadDB - загрузка базы данных из файла БД, к которому имеется путь 
 *   	% saveDB - сохранить имеющуюся БД в файл БД по адресу path
 *   	% addUser - добавление нового пользователя в БД системы
 *	% removeUser - удаления пользователя из БД системы
 *	% getUsers - получение полного списка пользователей в БД
*/
class CSDB
{
	vector<User> _userList; 
	UserStream _userStream;
	char *_currentDB;			// Ссылка на текущую, обрабатываемую БД

	void setNewDB(char *path) {
		_userList = _userStream.read(path); // считать с БД вектор (список) пользователей

		_currentDB = new char[strlen(path)+1];
		strncpy(_currentDB, path, strlen(path)+1);
	}
public:
	CSDB(char *path)
	{
		_userList.clear();				
		setNewDB(path);
	}

	CSDB()	// Инициализация пустой БД
	{
		_userList.clear();				
		_currentDB = NULL;
	}

	void loadDB(char *path)
	{
		_userList.clear();				
		setNewDB(path);
	}

	void saveDB()	
	{
		if(!_currentDB) {
			cerr << "Нет ссылки на файл БД!\n"; 
			return;
		}

		if(!_userStream.write(_currentDB, _userList)) // Сохранение БД
			return;

	}

	void addUser(const char name[NAME_SIZE], string password)
	{
		User tempUser;						// бланк регистрации	
		
		tempUser.setName(name);			//попробовать tempUser(name,password)
		tempUser.setPassword(password);
		_userList.push_back(tempUser);
	}

	int removeUser(char name[NAME_SIZE])
	{
		return 0;
	}

	vector<User> getUsers()
	{
		return _userList;
	}

	~CSDB(){				// Деструктор БД
		if(_currentDB) {
			saveDB();
			delete [] _currentDB;
		}
	}
};

class Application {
	CSDB userCSDB;
	char *pathToDB;
public:
	void init();
	void start();
	void dispose();
};

void Application::init()
{
	pathToDB = (char *) DB_PATH;
	userCSDB.loadDB(pathToDB);
}

void Application::start()
{
/*
	userCSDB.addUser("NeX", "123");
	userCSDB.addUser("Vitya", "3123");
	userCSDB.addUser("Klimov", "GeohotLohIPidor");
	userCSDB.addUser("TrueProger", "YaGeohot");
*/
	vector<User> users = userCSDB.getUsers();

	for(unsigned int i = 0; i < users.size(); i++)
		cout << users[i].getName() << endl;

}

void Application::dispose()
{
	userCSDB.saveDB();
}

int main()
{
	Application appl;

	appl.init();
	appl.start();
	appl.dispose();

	return 0;
}
