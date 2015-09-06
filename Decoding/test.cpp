#include "encode_manager.h"
#include <iostream>
#include <fstream>

using namespace std;

void test1(){
	ofstream logfile("test_log.txt");
	int chennel;
	for (int i = 1; i <= 144; i++){
		chennel = insert_cod::encode(i);
		cout << chennel << '\t';
		logfile << chennel<< '\t';
		if (i % 10 == 0){
			logfile << endl;
		}
	}
	logfile.close();
	getchar();
}

class basic
{
public:
	
	virtual void method() {
		cout << "basic";
	};

private:
	static int static_int;
};

class advanced :public basic
{
public:
	void method(){
		cout << "advanced";
	}
};

void test2(basic *tmp){
	tmp->method();
}

void main(){
	basic a;
	advanced b;
	test2(&b);
	getchar();
}

