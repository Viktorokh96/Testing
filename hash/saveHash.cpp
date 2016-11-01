#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

#define HKEY 2641761653
#define NAME_SIZE 41
typedef unsigned long int hashData;
class Hash {
	const hashData _hashkey;
	const hashData _hashValue;

	hashData hash( string item )
	{
		hashData hdret = 0;
		const char *txtch = item.c_str();
		unsigned char acc = (char) HKEY;

		for(; *txtch; txtch++) {
			const char *tmpch = txtch;
			for (; *tmpch; tmpch++)
				acc ^= *tmpch;
			hdret ^= (HKEY * acc);
			acc = (char) hdret;
		}

		return hdret;
	}

	hashData readHashKey(istream &stream) { 
		hashData retVal;
		stream.read(reinterpret_cast<char*>(&retVal),sizeof(hashData));

		return retVal;
	}
	hashData readHashValue(istream &stream) {
		hashData retVal;
		stream.read(reinterpret_cast<char*>(&retVal),sizeof(hashData));

		return retVal;
	}
public:
	Hash(string passw) : _hashkey( HKEY ), _hashValue( hash(passw) )
	{
	}

	Hash ( istream &stream ) : _hashkey( readHashKey(stream) ),
     		_hashValue( readHashValue(stream) ) 
	{
       	}
	
	// Перегрузка конструктора Hash
	Hash(Hash &hashParam) : _hashkey( hashParam._hashkey ), _hashValue( hashParam._hashValue )
	{
	}

	hashData hashPassword ( string passw )
	{
		return hash(passw);
	}
	bool hashCompare( string passw )
	{
		return (_hashValue == hash(passw));
	}

	friend ostream &operator<< (ostream &stream, Hash hashObj);
	istream &operator>> (istream &stream);
};

ostream &operator<< (ostream &stream, Hash hashObj) 
{
	stream.write(reinterpret_cast<const char*>(&hashObj._hashkey),sizeof(hashObj._hashkey));
	stream.write(reinterpret_cast<const char*>(&hashObj._hashValue),sizeof(hashObj._hashValue));

	return stream;
}

class User
{
	char _name[NAME_SIZE];
	Hash *_password;
public:
	// Конструктор пользователя
	User(){ _password = NULL; _name[0] = '\0'; }
	// Конструктор копий пользователя
	User(const User &user) { 
		strncpy(_name, user._name, NAME_SIZE);
	       	_password = new Hash(*user._password); 
	}	
	// Деструктор пользователя
	~User(){ if(_password) delete _password; }
	// Установка имени пользователя
	void setName(const char name[NAME_SIZE]) { strncpy(_name, name, NAME_SIZE); }
	// Получение имени пользователя
	string getName(){ 
		string retName = _name;
		return retName; 
	}
	// Установка нового пароля для пользователя
	void setPassword(string passw) {
		if(_password) delete _password;
	 	_password = new Hash(passw); 
	}
	// Сравнение имеющегося пароля с введённым
	bool comparePassw(string passw) { 
		if(_password) return _password->hashCompare(passw); 
		else return false;
	}
		
	friend ostream &operator<< (ostream &stream, User user);
	friend istream &operator>> (istream &stream, User &user);
};

istream &operator>> (istream &stream, User &user)
{
	if(user._password)
		delete user._password;

	stream.read(reinterpret_cast<char*>(user._name),sizeof(user._name));
	user._password = new Hash(stream);

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
	User someone;
	string password;

	someone.setName("Alex");
	someone.setPassword("Alex201233");

	ofstream file("./test.txt", ios_base::binary);
	
	file << someone << endl;
	
	file.close();

	ifstream input("./test.txt", ios_base::binary);

	input >> someone;
	
	cin >> password;
	if(someone.comparePassw(password))  cout << "Вы прошли" << endl;
	else cout << "Иди вон" << endl;

	input.close();

	return 0;
}
