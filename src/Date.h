/*
 * Date.h
 *
 *  Created on: Nov 18, 2017
 *      Author: lapoi
 */

#ifndef DATE_H_
#define DATE_H_

using namespace std;

class Date{

public:
	Date(unsigned short year);
	Date(unsigned short year, unsigned short month, unsigned short day);
protected:

private:
	unsigned short year;
	unsigned short month;
	unsigned short day;
};



#endif /* DATE_H_ */
