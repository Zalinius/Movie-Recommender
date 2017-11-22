#include "Date.h"
#include <locale>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<unsigned char> Date::delims {' ', '-', '\0'};
const unsigned short MIN_YEAR = 1888;



Date::Date()
: year(0), month(0), day(0)
{
}

Date::Date(unsigned short year)
: year(validateYear(year)), month(0), day(0)
{
}

Date::Date(unsigned short year, unsigned short month, unsigned short day)
: year(validateYear(year)), month(validateMonth(month)), day(validateDay(day))
{
}

Date::Date(string dateString)
{
	parseDate(dateString);
}

Date::~Date(){
}

unsigned short Date::validateYear(unsigned short draftYear){
	if(draftYear < MIN_YEAR)
		return 0;
	else
		return draftYear;
}
unsigned short Date::validateMonth(unsigned short draftMonth){
	if(draftMonth > 12)
		return 0;
	else
		return draftMonth;
}
unsigned short Date::validateDay(unsigned short draftDay){
	if(draftDay > 31)
		return 0;
	else
		return draftDay;
}

void Date::parseDate(const string& dateString){
	stringstream dateStringStream(dateString, ios_base::in);
	dateStringStream.imbue(locale(dateStringStream.getloc(), new CustomCType(delims, false)));

	unsigned short temp;
	if(dateStringStream >> temp){
		year = validateYear(temp);
		if(dateStringStream >> temp){
			month = validateMonth(temp);

			if(dateStringStream >> temp){
				day = validateDay(temp);
			}
			else
				day = 0;
		}
		else{
			month = 0;
			day = 0;
		}
	}
	else{
		year = 0;
		month = 0;
		day = 0;
	}
}

ostream & operator<< (ostream& os, const Date& d){
	string debugString = "";

	if(d.year != 0){
		debugString += to_string(d.year);
		if(d.month != 0 && d.day != 0){
			debugString += "-";
			if(d.month < 10)
				debugString += "0";
			debugString += to_string(d.month);
			debugString += "-";
			if(d.day < 10)
				debugString += "0";
			debugString += to_string(d.day);
		}
	}
	else
		debugString += "n/a";
	os << debugString;

	return os;
}

/*
int main(){
	Date d1("2012");
	Date d2(2020, 8, 12);
	Date d3("2010 -- -- - 05 -- - 23-");
	Date d4;
	Date d5("");
	cout << "Testing!" << endl;;
	cout << "Date 1: " << d1 << endl;
	cout << "Date 2: " << d2 << endl;
	cout << "Date 3: " << d3 << endl;
	cout << "Date 4: " << d4 << endl;
	cout << "Date 5: " << d5 << endl;

	return 0;
}
*/
