/*
 * Term.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: lapoi
 */

#include "Term.h"

/**
 * The Constructor for the Term class
 *  @param term The actual stored word. It should be in lower_case.
 */
Term::Term(string term)
:term(term)
{
	documentFrequency = 1;
}

const string& Term::getWord() const{
	return term;
}

void Term::incrementDocumentFrequency() const{
	++documentFrequency;
}

unsigned short Term::getDocumentFrequency() const{
	return documentFrequency;
}

/**
 * Two terms are equal if they contain the same word.
 */
const bool operator== (const Term & left, const Term & right){
	return(left.term == right.term);
}

/**
 * A term comes before another if it's word comes before the other's word alpha-numerically.<br>
 * Creates a strict, weak ordering system for Term objects.
 * @param left The term object on the left of the operator.
 * @param right The term object on the right of the operator.
 * @return True if the left Term precedes the right Term.<br>
 * False otherwise.
 */
const bool operator< (const Term & left, const Term & right){
	return(left.term < right.term);
}

/**
 * An overloading of the insertion operator for debugging purposes.<br>
 * Inserts information about the word and document frequency into the stream.
 * @param os The output stream.
 * @param t The term object being inserted into the stream.
 * @return The updated output stream.
 */
ostream &operator<< (ostream &os, const Term & t){
	os << t.term << " shows up in ";
	os << t.documentFrequency;
	os << " documents\n";
	return os;

}
