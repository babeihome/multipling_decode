// Coding method generator


//define encode and 

class chennel_inf
{

};

class fire_strip
{

};

class hardware_inf
{

};

class cod_gen
{
public:

	virtual int encode(int strip);	//encode method, input id of strip, return channel id
	virtual fire_strip decode(chennel_inf hitted);	//decode method, input hitted chennel id, return decoded information

private:

	int strip_check(int strip);

};

/*
class display_mod
{
	typedef int(cod_gen::encode *pFunc)(int strip);
	pFunc = &insert_cod::encode;
	void show_encode_map(pFunc,);
};
*/

class insert_cod : public cod_gen
{
public:
	
	//start from line 5, such as 1,10,2,11,3,10,4,.......10,11
	static int encode(int strip);
	static fire_strip decode(chennel_inf hitted);

private:
	static int strip_check(int strip);
	static const int strip_displace = 2 * 4 * 4 + 4;	//36, 2k^2+k k=4
};