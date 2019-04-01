#include <string>
#include <iostream>
using namespace std;

// sum of items
template<typename T>
T adder(T v) {
	cout << __PRETTY_FUNCTION__ << "\n";
	return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
	cout << __PRETTY_FUNCTION__ << "\n";
	return first + adder(args...);
}


// pair-wise comparison for many pairs
template<typename T>
bool compare(T a, T b){
	return a==b;
}

template<typename T>
bool compare(T a){
	return false;
}

template<typename T, typename... Args>
bool compare(T a, T b, Args... args){
	return a==b && compare(args...);
}



int main() {
	string s1 = "x", s2 = "aa", s3 = "bb", s4 = "yy";
	string ssum = adder(s1, s2, s3, s4);
	cout << ssum << "\n";

	cout << "compare output is "<<compare(1.5,1.5,0,0,"hello","hello")<<"\n";

	return 0;
}


