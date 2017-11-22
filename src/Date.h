#ifndef DATE_H_
#define DATE_H_

#include <iostream>
#include "CustomCType.h"

using namespace std;

class Date{

public:
	Date();
	Date(unsigned short year);
	Date(unsigned short year, unsigned short month, unsigned short day);
	Date(string dateString);
	~Date();

	friend ostream & operator<< (ostream& os, const Date& d);
protected:
private:
	unsigned short year;
	unsigned short month;
	unsigned short day;

	unsigned short validateYear(unsigned short draftYear);
	unsigned short validateMonth(unsigned short draftMonth);
	unsigned short validateDay(unsigned short draftDay);

	static vector<unsigned char> delims;
	void parseDate(const string& dateString);
};

#endif /* DATE_H_ */
