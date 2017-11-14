/*
 * Sentence.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#include "Sentence.h"
#include <iostream>
#include <string>

using namespace std;

/**
 * The primary constructor for the Sentence class.
 * @param sentence The string to be turned into a sentence. It is assumed it is a valid sentence.
 * @param allowCopies Whether or not duplicates should be kept when tokenizing the words of the sentence.
 * @param pos The sentence's original position in a text document. Should be positive.
 */
Sentence::Sentence(const string& sentence, bool allowCopies, unsigned int pos)
: IndexItem(sentence, allowCopies), pos(pos), doc(0)
{
	//cout << "Sentence Constructed" << endl;
}

Sentence::~Sentence(){
}

/**
 * @return The sentence's original position in a text document.
 */
unsigned int Sentence::getPos() const{
	return pos;
}

/**
 * @return The number of words in the sentence.
 */
string::size_type Sentence::size() const{
	return getTokens().size();
}

/**
 * @param docNumber The document number this sentence was found in.
 */
void Sentence::setDoc(unsigned int docNumber){
	doc = docNumber;
}

/**
 * The polymorphic function used in IndexItem's << operator
 * @return The original file's name, the number of sentences and the text.
 */
const string Sentence::toString() const{
	//cout << "The correct function is being called, size: " << content().size() << endl;
	string debugString;
	debugString += "#";
	debugString += to_string(pos);
	debugString += " ";
	debugString += content();
	return debugString;
}

/**
 * Overriding of the < operator for use in <algorithm> functions
 * A Sentence precedes another if it is from and earlier Document.
 * If Documents are identical, then a Sentence precedes another if it comes earlier in said Document
 * @param left The left Sentence to be compared
 * @param right The right Sentence to be compared
 * @return True if the left Sentence comes before the right Sentence. False otherwise.
 */
bool operator< (const Sentence & left, const Sentence & right){
	if(left.doc != right.doc)
		return left.doc < right.doc;
	else
		return left.pos < right.pos;
}
