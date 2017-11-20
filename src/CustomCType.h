/*
 * CustomCType.h
 *
 *  Created on: Nov 19, 2017
 *      Author: lapoi
 */

#ifndef CUSTOMCTYPE_H_
#define CUSTOMCTYPE_H_

#include <locale>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>

using namespace std;

class CustomCType : public ctype<char>
{
	mask my_table[table_size];
public:
	CustomCType(const vector<unsigned char>& delimiters, bool noSpace, size_t refs = 0)
	: ctype<char>(&my_table[0], false, refs)
	  {
		copy_n(classic_table(), table_size, my_table);

		if(noSpace)
			my_table[' '] ^= ctype_base::space;
		for(vector<unsigned char>::const_iterator it = delimiters.begin(); it != delimiters.end(); ++it){
			my_table[*it] = (mask)space;
		}
	  }
};





#endif /* CUSTOMCTYPE_H_ */
