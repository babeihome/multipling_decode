#include "encode_manager.h"
#include <math.h>
#include <iostream>

#define DEBUG 0

test_mod::test_mod() {
	cout << "Warning: No specific coding method binded";
}

test_mod::test_mod(basic_cod_gen *a) {
	cod = a;
	Set_logpath("logfile.txt");
}

void test_mod::Set_cod(basic_cod_gen *tested_method) {
	cod = tested_method;
}

void test_mod::Set_logpath(const char* filepath) {
	logfile.open(filepath);
}

test_mod::~test_mod() {
	logfile.close();
}

bool test_mod::check_cod() {
	return true;
}

void test_mod::display() {
	if (!check_cod()) {
		cout << "Error: No specific coding method be referenced";
		return;
	}
	int chennel = -1;
	for (int i = 1; i <= 144; i++) {
		chennel = cod->encode(i);
		cout << chennel << '\t';
		logfile << chennel << '\t';
		if (i % 10 == 0) {
			logfile << endl;
		}
	}
}