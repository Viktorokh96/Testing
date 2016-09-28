#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

#define HASH_KEY 12098391

#define array_size(a) 	(sizeof(a)/sizeof(a[0]))

typedef unsigned long int hashData;
hashData hash (const char *password)
{
	hashData hdret = HASH_KEY;

	hashData htable[] = {
		0x51dbcca3, 0x2421fbca,
		0x192bacad, 0x1f4bfcca,
		0xbd4281ca, 0x2421feca,
		0x8239cadb, 0x34212eca,
		0xbbaca314, 0x2221fbca,
		0x521981ca, 0x2421fbca,
		0x412ba11e, 0x329cea12,
		0xe13ee9ba, 0x11591828,
		0x192bacac, 0x15812899,
		0x51dec1a3, 0xba9c8a88,
		0x8339badb, 0x1994889b,
		0xb44281ca, 0x2ff1f3ca,
		0x52e981da, 0x2121fe9a,
		0xabaca314, 0x8451eb8a,
		0xe1e5e9ba, 0x2441abc0,
		0x413ba11e, 0x4441cb1a
	};
	
	const char *ch = password;

	for(; *ch ; ch++ ) {
		char tch = *ch;
		for(unsigned int i = 0; i < (sizeof(hashData)*8); i++) {
			hdret ^= ((tch) & (1 << (i % 8))) ? 
				htable[tch % array_size(htable)] : htable[array_size(htable) - (tch % array_size(htable))];
			tch ^= hdret % tch;
		}
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


//	for(it1 = usertable.begin(); it1 != usertable.end(); it1++)
//		cout << "passw:\t" << (*it1).getHashed() << endl;

	return 0;
}
