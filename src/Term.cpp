/*
 * Term.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: lapoi
 */

#include "Term.h"

/**
 * The Constructor for the Term class
 *  @param term The actual stored word
 *  @param docNo The file the word was found in
 *  @param fileAmount the amount of files being processed, for sizing the vectors
 */
Term::Term(string term, unsigned int docNo, size_t fileAmount)
:term(term)
{
	documentFrequency = 1;
	termFrequencies.resize(fileAmount);
	weight.resize(fileAmount);
	for(vector<unsigned int>::iterator it = termFrequencies.begin(); it != termFrequencies.end(); ++it){
		*it = 0;
	}

	termFrequencies.at(docNo) = 1;
}

/**
 * Two terms are equal if they contain the same word.
 */
const bool operator== (const Term & left, const Term & right){
	return(left.term == right.term);
}

/**
 * A term comes before another if it's word comes before the other's word alphabetically.
 */
const bool operator< (const Term & left, const Term & right){
	return(left.term < right.term);
}

ostream &operator<< (ostream &os, const Term & t){
	os << t.term << " shows up in ";
	os << t.documentFrequency;
	os << " documents\n";
	return os;

}
