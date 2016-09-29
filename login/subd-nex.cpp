#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>

#define HKEY "c.h2#?!zj3"
#define NAME_MAXSIZE 15
#define WORD_SIZE 4
#define IN_FILE "users.dat"

using namespace std;
typedef string hashData;
class Hash {
	const hashData _hashkey;
	hashData hash( string item )
	{
		const char *c = item.c_str();
		char acc[8] = {0};	// аккумулятор
		char wordBuf[WORD_SIZE] = {0};
		unsigned int itemLength = item.length();
		unsigned int i = 0, j = 0, k = 0;
		hashData hdreturn = _hashkey;
		unsigned int hdLength = hdreturn.length();
		
		i = itemLength % WORD_SIZE; // генерация отступа от первого байта, WORD_SIZE - количество символов шифруемых за один раз
		for(j = 0; j < itemLength; j++, c++)
		{
			if((j % WORD_SIZE) == (WORD_SIZE - 1))
			{
				//wordBuf[j%WORD_SIZE] = *c; // буфер заполнен, теперь из него нужно передать информацию в аккумулятор, зашифровав её в нём.
				wordBuf[j%WORD_SIZE] = *c ^ (j|hdreturn[j]);
				for(k = 0; k<WORD_SIZE; k++) // для записи всего слова
				{
					acc[i+k] ^= ~wordBuf[k];
				}
				memset(wordBuf, 0, sizeof(wordBuf)); // очищаем массив символов слова
			}
			else
				wordBuf[j%WORD_SIZE] = *c << (j%8); // усовершенствовал, чтобы пароль шифр двух букв рядом ab и ba сильно отличался
			
			if(i < (itemLength - WORD_SIZE)) // отступ от первого байта аккумулятора
				++i;
			else
				i = 0;								
		}
		if(wordBuf[0]) // если что-то осталось в буфере (когда строка не была кратна WORD_SIZE)
		{				
			k = 0;
			do
			{
				acc[k] ^= ~wordBuf[k];
				k++;
			}while(wordBuf[k]); // проверяем на наличие остальных символов
		}
		
/* 		 Запись в цифровом виде с доп. шифровщиком */
		for(i = 0;i < itemLength; i++) 
		{
			j = i%sizeof(acc); // будет от 0 до 10 (размера hashData)
			for(k = 0; k<hdLength; k++)
			{
				hdreturn[k] ^= acc[j];
				hdreturn[k] = ~hdreturn[k];
			}
					
			k = (i+j)%WORD_SIZE; // усложняем шифрование
			hdreturn[j] &= ~acc[j];
		}		
		return hdreturn;
	}
public:
	Hash() : _hashkey( HKEY )
	{
	}
	hashData hashPassword ( string passw )
	{
		return hash(passw);
	}
	bool hashCompare( hashData hd, string passw )
	{
		return (hd == hash(passw));
	}
};

class User
{
	string _name;
	hashData _password;
public:
	User(){}
	void setName(string name){ _name = name; }
	string getName(){ return _name; }
	void setPassword(hashData passw) { _password = passw; }
	hashData getPassword() { return _password; }
		
};

class UserStream
{
	User _userinfo;
public:
	vector <User> read(string path){
		string line;
		string parseline;
		const char *ch;
		vector <User> tempUsers;
		
		parseline.clear();
		
		ifstream file(path.c_str());
		while(!file.eof()) {
			getline(file, line);
			if (line == "") break;
			ch = line.c_str();
			while( *ch != ' ')
			{
				parseline.push_back(*ch);			// Считали имя
				ch++;
			}
			
			_userinfo.setName(parseline);
			parseline.clear();
			
			while( *ch == ' ') ch++;
			
			while( *ch != '\0')
			{
				parseline.push_back(*ch);			// Считали имя
				ch++;
			}
			
			_userinfo.setPassword(parseline.c_str());
			parseline.clear();
			
			tempUsers.push_back(_userinfo);
		}
		
		return tempUsers;
	}
	void write(string path, vector <User> users)
	{
		
	}
};

 class CSDB
 {
	vector<User> _userList; 
	Hash _hash;
	UserStream _parser;
public:
	CSDB(string path)
	{
		_userList.clear();
				
		_userList = _parser.read(path); // добавить элемент в конец вектора
	}
	int addUser(string login, string password)
	{
		User tempuser;						// бланк регистрации
		
		tempuser.setPassword( _hash.hashPassword(password) );
		tempuser.setName(login);
		
		_userList.push_back(tempuser);
		return 0;
	}
	int removeUser(string login)
	{
		return 0;
	}
	vector<User> getUsers()
	{
		return _userList;
	}
	~CSDB(){}
	
 };
 
 int main()
 {
	Hash hash;
	string passw;
	User user1;
	vector<User> userList;
	
	cin >> passw;
	user1.setPassword(hash.hashPassword(passw));
	cout << user1.getPassword() << endl;
//	CSDB csdb(IN_FILE);
//	userList = csdb.getUsers(); 
	
//	vector<User>::iterator iter = userList.begin();
//	for(;iter != userList.end(); ++iter)
//		cout << (*iter).getName() << " " << (*iter).getPassword() << endl;
	
	return 0;
 }