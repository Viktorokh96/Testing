#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

#define HASH_KEY 12098391
#define HTSIZE 	16

#define array_size(a) 	(sizeof(a)/sizeof(a[0]))

typedef unsigned long int hashData;
hashData hash (const char *password)
{
	hashData hdret = HASH_KEY;

	hashData htable[HTSIZE] = {
		0x51dbcca3,
		0x192bacad,
		0xbd4281ca,
		0x8239cadb,
		0xbbaca314,
		0x521981ca,
		0x412ba11e,
		0xe13ee9ba,
		0x192bacac,
		0x51dec1a3,
		0x8339badb,
		0xb44281ca,
		0x52e981da,
		0xabaca314,
		0xe1e5e9ba,
		0x413ba11e
	};
	
	const char *ch = password;

	for(; *ch ; ch++ ) {
		char tch = *ch;
		for(int i = 0; i < (sizeof(hashData)*8); i++) {
			hdret ^= ((tch) & (1 << (i % 8))) ? htable[tch % HTSIZE] : htable[HTSIZE - (tch % HTSIZE)];
			tch ^= hdret % tch;
		}
	}
	
	return hdret;
}

class User {
	const char *_passwString;
	hashData _password;
public:
	User( const char *orig, hashData passw ) { _passwString = orig; _password = passw; }
	const char *getOriginal() { return _passwString; }
	hashData getHashed()	{ return _password; }
};


int main()
{
	const char testStrArr[][120] = {"passw1", "passw2", "dska", "jnavk",
       				  	"qwerty", "sakd", "biwu",
				     	"hohmoh", "boqk", "nvqu"
				     	"chudenbobyknoBeniy", "3Ff2382",
       					"b", "bf", "bd", "doso",
					"dosd", "dosk", "asdo",
					"sa", "as", "ba", "ca",
					"aius", "asd", "disoa",
					"sdufoi", "dlsklfal", "iqoi"
					"qoq", "oqc", "qiwou", "hfbq",
					"wbq", "nvoqi", "woeiru", "oiqn"
					"oqwe", "niqze", "mizqt", "mniq",
					"aosbub","fsdbb","qwcq", "mown",
					"Bbgwu832_h", "obuib_32", "buu193b",
					"o2bisd", "bhrieh", "4hh203s",
					"oujqdo", "wigwb", "owiawoe", "wobqw"};

	vector<User> usertable;
	vector<User>::iterator it1, it2;

	for(int i = 0; i < array_size(testStrArr); i++) {
		User tempuser(testStrArr[i], hash(testStrArr[i]));
		usertable.push_back(tempuser);
	}

	for(it1 = usertable.begin(); it1 != usertable.end(); it1++) {
		hashData tempHash = (*it1).getHashed();
		for(it2 = usertable.begin(); it2 != usertable.end(); it2++)
			if (it1 != it2)
				if ((tempHash == (*it2).getHashed())) { 
					cout << "Обнаружено совпадение:" << endl;
					cout << "Login:" << (*it1).getOriginal() << "\tPassword:" << (*it1).getHashed() << endl;
					cout << "Login:" << (*it2).getOriginal() << "\tPassword:" << (*it2).getHashed() << endl;
				};
	}


	for(it1 = usertable.begin(); it1 != usertable.end(); it1++)
		cout << "passw:\t" << (*it1).getHashed() << endl;

	string str;
	while(true) {
		getline(cin, str);
		cout << hash(str.c_str()) << endl;
	}

	return 0;
}
