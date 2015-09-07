#include "encode_manager.h"
#include "code_method.h"
#include <iostream>
#include <fstream>

using namespace std;

void test1(){
	ofstream logfile("test_log.txt");
	int chennel;
	
	insert_cod ins;
	for (int i = 1; i <= 144; i++) {
		chennel = ins.encode(i);
		cout << chennel << '\t';
		logfile << chennel << '\t';
		if (i % 10 == 0) {
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

	insert_cod cod;
	test_mod display_test(&cod);
	display_test.display();
	/*
	basic a;
	advanced b;
	basic *ap;
	ap = &b;
	test2(ap);
	*/
	getchar();
}

