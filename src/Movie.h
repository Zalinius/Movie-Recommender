/*
 * movie.h
 *
 *  Created on: Nov 18, 2017
 *      Author: lapoi
 */

#ifndef MOVIE_H_
#define MOVIE_H_

using namespace std;

class Movie : public IndexItem{

public:
	Movie(const string content, bool allowCopies, const string name, unsigned int iD, Date releaseDate);
	virtual ~Movie() override;

	virtual string::size_type size() const override;

	const string getName() const;
	unsigned int getID() const;
	const Date getReleaseDate() const;
protected:

private:
	string name;
	unsigned int iD;
	Date releaseDate;

	virtual const string toString() const override;
};



#endif /* MOVIE_H_ */
