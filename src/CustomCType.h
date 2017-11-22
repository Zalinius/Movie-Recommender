#ifndef CUSTOMCTYPE_H_
#define CUSTOMCTYPE_H_

#include <locale>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>

using namespace std;

/**
 * This special class is used to create custom delimiters objects to imbue into input streams.
 */
class CustomCType : public ctype<char>
{
	mask my_table[table_size];
public:
	/**
	 * @param delimiters
	 * @param noSpace
	 */
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
