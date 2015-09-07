#pragma once
//declarate specfic code method here by users
#include "encode_manager.h"
#include <iostream>

using namespace std;

class insert_cod : public basic_cod_gen
{
public:

	//start from line 5, such as 1,10,2,11,3,10,4,.......10,11
	int encode(int strip);
	fire_strip decode(chennel_inf hitted);

private:
	int strip_check(int strip);
	const int strip_displace = 2 * 4 * 4 + 4;	//36, 2k^2+k k=4
};