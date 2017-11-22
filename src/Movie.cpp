#include "Movie.h"
#include "Date.h"
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

vector<unsigned char> Movie::delims {'\t'};

/**
 * The Primary constructor for Movie, using a special tvsString
 * @param tsvString A tab delimeted string containing: Id, junk, Title, Release Date.
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

/**
 * Retrieves and stores the information on a Movie object from a special tvsString
 * @param tsvString A tab delimeted string containing: Id, junk, Title, Release Date.
 *
 */
void Movie::parseMovie(const string& tvsString){

	stringstream dateStringStream(tvsString, ios_base::in);

	//Imbuing the stringstream with custom delimiters
	dateStringStream.imbue(locale(dateStringStream.getloc(), new CustomCType(Movie::delims, true)));

	string garbage; //For catching the \t character
	string dateString; //For catching the special date string

	dateStringStream >> iD;
	dateStringStream >> garbage;
	dateStringStream >> title;
	dateStringStream >> dateString;
	releaseDate = Date(dateString);

}

/**
 * @return The size of the movie description in characters.
 */
string::size_type Movie::size() const{
	return content().size();
}

/**
 * The polymorphic function used in IndexItem's << operator
 * @return The Movie's ID, title, release date and summary, in a nicely formatted string.
 */
const string Movie::toString() const{
	stringstream debugString;
	debugString << "ID:" << setw(10) << left << to_string(iD) + ":";
	debugString << title;
	debugString << ", ";
	debugString << releaseDate;
	debugString << ".\n\t";
	debugString << content();
	debugString << "\n";

	return debugString.str();
}

const string Movie::getName() const{
	return title;
}

unsigned int Movie::getID() const{
	return iD;
}

const Date Movie::getReleaseDate() const{
	return releaseDate;
}


/**
 * Compares movie ID for order
 */
bool operator < (const Movie &left, const Movie &right){
	return left.iD < right.iD;
}

/**
 * Compares movie titles for equality.
 */
bool operator == (const Movie &left, const Movie &right){
	string lowerLeft = left.title;
	string lowerRight = right.title;
	transform(lowerLeft.begin(), lowerLeft.end(), lowerLeft.begin(), ::tolower);
	transform(lowerRight.begin(), lowerRight.end(), lowerRight.begin(), ::tolower);
	return lowerLeft == lowerRight;
}

bool Movie::operator () (const Movie* m) const{
	return *this == *m;
}
