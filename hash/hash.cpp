#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <cstring>
using namespace std;

#define HKEY 0xACA0CFE3
#define array_size(a) 	(sizeof(a)/sizeof(a[0]))

typedef unsigned long int hashData;
hashData hash (const char *password)
{
	hashData hdret = 0;
	const char *txtch = password;
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

class User {
	string _passwString;
	hashData _password;
public:
	User( string orig, hashData passw ) { _passwString = orig; _password = passw; }
	string getOriginal() { return _passwString; }
	hashData getHashed()	{ return _password; }
};


int main(int argc, char *argv[])
{
	if (argc < 2) { 
		cout << "Usage:" << argv[0] << " + password file" << endl;
		return -1;
       	}

	vector<User> usertable;
	vector<User>::iterator it1, it2;

	ifstream in(argv[1]);
	string tempstr;
	while (!in.eof()) {
		in >> tempstr;
		if(tempstr == "") break;
		User tempuser( tempstr, hash( tempstr.c_str() ));
		usertable.push_back(tempuser);
	}
/*
	for(it1 = usertable.begin(); it1 != usertable.end(); it1++) {
		hashData tempHash = (*it1).getHashed();
		for(it2 = usertable.begin(); it2 != usertable.end(); it2++)
			if (it1 != it2)
				if ((tempHash == (*it2).getHashed())) { 
					cout << "Обнаружено совпадение:" << endl;
					cout << "Original:" << (*it1).getOriginal() << "\tPassword:" << (*it1).getHashed() << endl;
					cout << "Original:" << (*it2).getOriginal() << "\tPassword:" << (*it2).getHashed() << endl;
				};
	}
*/

	ofstream out("hashed.txt");
	for(it1 = usertable.begin(); it1 != usertable.end(); it1++)
		out << (*it1).getHashed() << endl;
	out.close();

	return 0;
}
