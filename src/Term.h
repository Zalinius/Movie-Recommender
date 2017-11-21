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
#include <vector>
#include <iostream>

class Term{

public:
	Term(string term, unsigned short docNo, size_t fileAmount);

	string term; //The actual word being represented

	unsigned short documentFrequency; //The document frequency
	vector<unsigned short> termFrequencies; //The termFrequency per document, with the index being the Doc#

	void incrementDocumentFrequency() const;	//doesn't change the ordering on the object in the set

	friend const bool operator== (const Term & left, const Term & right);
	friend const bool operator< (const Term & left, const Term & right);
	friend ostream &operator<< (ostream &os, const Term & right);

private:

};



#endif /* TERM_H_ */
