#ifndef MOVIE_H_
#define MOVIE_H_

#include "IndexItem.h"
#include "Date.h"
#include <iostream>
#include <string>

using namespace std;

/**
 * An IndexItem derived class used to store information about a Movie.<br>
 * It stores its Wikipedia ID, its title, its release date and its summary.<br>
 */
class Movie : public IndexItem{

public:
	Movie(string tsvString);
	Movie(const string content, const string title, unsigned int iD, Date releaseDate);
	virtual ~Movie() override;

	virtual string::size_type size() const override;

	const string getName() const;
	unsigned int getID() const;
	const Date getReleaseDate() const;

	friend bool operator < (const Movie &left, const Movie &right);
	friend bool operator ==(const Movie &left, const Movie &right);
	bool operator() (const Movie* m) const;

protected:

private:
	string title;
	unsigned int iD;
	Date releaseDate;

	virtual const string toString() const override;

	static vector<unsigned char> delims;
	void parseMovie(const string& tvsString);
};



#endif /* MOVIE_H_ */
