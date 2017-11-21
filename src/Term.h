/*
 * Term.h
 *
 *  Created on: Oct 17, 2017
 *      Author: lapoi
 */

#ifndef TERM_H_
#define TERM_H_

using namespace std;

#include <string>
#include <iostream>

/**
 * A wrapper class for considering terms. It contains the word in question, as well as its document frequency.<br>
 * Term objects follow a strict weak ordering determined by the alpha-numerical order of their words.
 */
class Term{

public:
	Term(string term);

	const string& getWord() const;

	void incrementDocumentFrequency() const;	//doesn't change the ordering on the object in the set
	unsigned short getDocumentFrequency() const;


	friend const bool operator== (const Term & left, const Term & right);
	friend const bool operator< (const Term & left, const Term & right);
	friend ostream &operator<< (ostream &os, const Term & right);

private:
	string term; //The actual word being represented

	/**
	 * This specific member variable is mutable, as we use Term objects in const datastructures, such as set.<br>
	 * As they are const for ordering reasons, it is ok to mark documentFrequency mutable, as it does not affect the ordering of a Term object.
	 * @see Set
	 */
	mutable unsigned short documentFrequency; //The document frequency
};

#endif /* TERM_H_ */
