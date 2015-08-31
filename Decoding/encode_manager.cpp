#include "encode_manager.h"
#include <math.h>
#include <iostream>

#define DEBUG 0

int insert_cod::encode(int strip)
{
	if (strip_check(strip) == 0){
		return 0;
	}

	int line;
	int displacement;
	int chennel;

	line = (sqrt((float)(strip_displace*8+1 + 8 * strip)) - 1) / 4 - 0.0001; //get which previous line are strip on   Warning: Careful when strip at right edge of line
	displacement = strip + strip_displace -(2 * ((int)line + 1)*(int)line - (int)line);	//displacement = strip - 2*line^2+line
	if (displacement % 2 == 1){
		chennel = (displacement + 1) / 2;
		if (displacement == 4 * line + 3){
			chennel++;
		}
	}
	else if (displacement % 4 == 0){
		chennel = (line + 1) * 2 + 1;
	}
	else
		chennel = (line + 1) * 2;
	if (DEBUG){
		std::cout << std::endl;
		std::cout << "strip:\t" << strip << std::endl;
		std::cout << "line:\t" << line << std::endl;
		std::cout << "displacement:\t" << displacement << std::endl;
		
	}
	return chennel;
}

int insert_cod::strip_check(int strip){
	return 1;
}