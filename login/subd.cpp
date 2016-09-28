#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>

#define HKEY 2641761653
#define NAME_MAXSIZE 15
#define IN_FILE "users.dat"

using namespace std;
typedef long int hashData;
class Hash {
	const hashData _hashkey;
	hashData hash( string item )
	{
		const char *c = item.c_str();
		char acc = 0;	// аккумулятор
		unsigned int i = 0;
		hashData hdret = _hashkey;
		
		for(;*c; c++)
			acc ^= *c;
		
		for(;i < sizeof(hashData); i++) {
				hdret ^= ((acc*i) << (i*8));
		}
		
		return hdret;
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
			
			_userinfo.setPassword(atoi(parseline.c_str()));
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
	vector<User> userList;
	
	CSDB csdb(IN_FILE);
	userList = csdb.getUsers(); 
	
	vector<User>::iterator iter = userList.begin();
	for(;iter != userList.end(); ++iter)
		cout << (*iter).getName() << " " << (*iter).getPassword() << endl;
	
	return 0;
 }