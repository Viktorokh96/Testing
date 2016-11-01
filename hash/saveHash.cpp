#include <iostream>
using namespace std;

#define HKEY 2641761653

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
public:
	Hash(string passw) : _hashkey( HKEY ), _hashValue( hash(passw) )
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
};


class User
{
	string _name;
	Hash *_password;
public:
	// Конструктор пользователя
	User(){ _password = NULL; _name.clear(); }
	// Конструктор копий пользователя
	User(const User &user) { _name = user._name; _password = new Hash(*user._password); }	
	// Деструктор пользователя
	~User(){ if(_password) delete _password; }
	// Установка имени пользователя
	void setName(string name){ _name = name; }
	// Получение имени пользователя
	string getName(){ return _name; }
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
};

ostream &operator<< (ostream &stream, User user)
{
	stream << user._name;
	return stream;
}

int main()
{
	User someone;
	string password;

	someone.setName("Alex");
	someone.setPassword("Alex201233");
	
	cin >> password;
	cout << someone.comparePassw(password) << endl;

	cout << someone << endl;

	return 0;
}
