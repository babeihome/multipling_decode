// Framework of system
#include <iostream>
#include <fstream>

using namespace std;

#ifndef FRAMEWORK
#define FRAMEWORK
class chennel_inf
{

};

class fire_strip
{

};

class hardware_inf
{

};


//basic code method generator
class basic_cod_gen
{
public:

	virtual int encode(int strip) { return 0; }	//encode method, input id of strip, return channel id
	virtual fire_strip decode(chennel_inf hitted) {
		fire_strip result;
		return result;
	}	//decode method, input hitted chennel id, return decoded information

private:

	int strip_check(int strip) { return 0; }

};


class test_mod
{
public:

	//config setting function
	test_mod();
	~test_mod();
	test_mod(basic_cod_gen *cod_point);
	void Set_cod(basic_cod_gen *tested_method);
	void Set_logpath(const char* filepath);

	//test function
	void display();
	

private:
	bool check_cod();
	basic_cod_gen *cod;
	ofstream logfile;
};
#endif // !FRAMEWORK



