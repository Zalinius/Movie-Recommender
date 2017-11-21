/*
 * Movie.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: lapoi
 */

#include "Movie.h"
#include "Date.h"
#include <iomanip>
#include <sstream>

using namespace std;

vector<unsigned char> Movie::delims {'\t'};

/**
 * @param tsvString A tab delimeted string containing: Id, meow, Title, Release Date,
 */
Movie::Movie(string tsvString)
:IndexItem("", true)
{
	parseMovie(tsvString);
}

Movie::Movie(const string content , const string title, unsigned int iD, Date releaseDate)
: IndexItem(content, true), title(title), iD(iD), releaseDate(releaseDate)
{

}

Movie::~Movie(){

}

void Movie::parseMovie(const string& tvsString){

	stringstream dateStringStream(tvsString, ios_base::in);

	dateStringStream.imbue(locale(dateStringStream.getloc(), new CustomCType(Movie::delims, true)));

	string garbage;
	string dateString;

	dateStringStream >> iD;
	dateStringStream >> garbage;
	dateStringStream >> title;
	dateStringStream >> dateString;
	releaseDate = Date(dateString);

}


string::size_type Movie::size() const{
	return content().size();
}

const string Movie::toString() const{
	stringstream debugString;
	debugString << setw(10) << left << to_string(iD) + ":";
//	debugString << ":";
	debugString << title;
	debugString << ", ";
	debugString << releaseDate;

	return debugString.str();
}

bool operator < (const Movie &left, const Movie &right){
	return left.iD < right.iD;
}

/*
int main(){
	Movie m1("Rawr", "Dawn of the Apes", 12345, Date("2010-04-12"));
	Movie m2("12345	mksk	Dawn of the Apes	2010 - 04- 12");
	Movie m3("34954835	/m/0j45br8	Ronan's Escape	2010-02-15		16.0	");
	Movie m4("32289648	/m/0gxzbv9	Biktima			90.0	");
	cout << m1 << endl;
	cout << m2 << endl;
	cout << m3 << endl;
	cout << m4 << endl;

}
*/
