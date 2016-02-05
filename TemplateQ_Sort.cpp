#include <iostream>
using namespace std;

int i_temp[15] = {6,7,4,32,3,7,8,34,11,22,31,12,16,18,5};
double d_temp[15] = {6.3,7.1,4.5,32.6,3.2,7.9,8.1,34.2,11.8,22.5,31.9,12.3,16.1,18.7,5.4};
long l_temp[15] = {6L,7L,4L,32L,3L,7L,8L,34L,11L,22L,31L,12L,16L,18L,5L};
char c_temp[15] = {'f','s','a','k','g','u','q','n','x','z','b','n','m','l','w'};

// вся соль в этом шаблоне
template <class T> void q_sort ( T *arg, int left, int right ) 
{
	int L = left;
	int R = right;
	T tmp;
	T m = (arg[L] + arg[R]) / 2; 				// опорный элемент
	while (L <= R) {
		while ((arg[L] < m) && (L < right)) L++;
		while ((arg[R] > m) && (R > left)) R--;
		if (L <= R) {
			tmp = arg[L];
			arg[L] = arg[R];
			arg[R] = tmp;
			L++; R--;
		}
	}
	
	// Рекурсия
	if (left < R) q_sort(arg,left,R);
	if (L < right) q_sort(arg,L,right);
}


int main()
{
	register int i;
	
	cout << "До сортировки: " << endl;
	
	for(i = 0 ; i < 15 ; i ++) cout << i_temp[i] <<' ';
	cout << endl;
	
	for(i = 0 ; i < 15 ; i ++) cout << d_temp[i] <<' ';
	cout << endl;
	
	for(i = 0 ; i < 15 ; i ++) cout << l_temp[i] <<' ';
	cout << endl;
	
	for(i = 0 ; i < 15 ; i ++) cout << c_temp[i] <<' ';
	cout << endl;
	cout << endl;
	
	q_sort(i_temp,0,14);
	q_sort(d_temp,0,14);
	q_sort(l_temp,0,14);
	q_sort(c_temp,0,14);
	
	cout << "После сортировки: " << endl;
	
	for(i = 0 ; i < 15 ; i ++) cout << i_temp[i] <<' ';
	cout << endl;
	
	for(i = 0 ; i < 15 ; i ++) cout << d_temp[i] <<' ';
	cout << endl;
	
	for(i = 0 ; i < 15 ; i ++) cout << l_temp[i] <<' ';
	cout << endl;
	
	for(i = 0 ; i < 15 ; i ++) cout << c_temp[i] <<' ';
	
	return 0;
}
