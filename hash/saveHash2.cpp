#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

#define HKEY 2641761653
#define NAME_SIZE 41
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
	Hash ( istream &stream ) : _hashKey( readHashKey(stream) ),	_hashValue( readHashValue(stream) ) 
	{
    }
	// Перегрузка конструктора Hash	
	Hash(Hash &hashParam) : _hashKey( hashParam._hashKey ), _hashValue( hashParam._hashValue )
	{
	}
	
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
	void setName(const char name[NAME_SIZE]) { strncpy(_name, name, NAME_SIZE); } // сделать динамический!
	// Получение имени пользователя
	string getName(){ 
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
	stream.write(reinterpret_cast<char*>(&user._name),sizeof(user._name));
	stream << *user._password;

	return stream;
}

int main()
{
	User *someone;
	string password;

	someone = new User("gogo", "123");

	ofstream file("./test.txt", ios_base::binary | ios_base::app);
	
	file << *someone << endl;
	
	file.close();

	ifstream input("./test.txt", ios_base::binary);

	input >> *someone;
	
	cin >> password;
	if(someone->comparePassword(password))  cout << "Вы прошли" << endl;
	else cout << "Иди вон" << endl;

	input.close();

	return 0;
}
