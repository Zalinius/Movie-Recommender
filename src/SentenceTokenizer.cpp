/*
 * SentenceTokenizer.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#include "SentenceTokenizer.h"
#include "Sentence.h"
#include "Stopword.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * The primary constructor for SentenceTokenizer objects
 * @param text The text to be tokenized into Sentence objects.
 * @param Whether or not copies are allowed. Default is True.
 */
SentenceTokenizer::SentenceTokenizer(const string text, bool allowCopies)
:AbstractTokenizer(text, tokenize(text), allowCopies)
{
	//cout << "Sentence Tokenizer Constructed" << endl;
}

SentenceTokenizer::~SentenceTokenizer(){
}

/**
 * The polymorphic function used in AbstractTokenizer's << operator
 * @return All the Tokenized Sentences, separated by newlines.
 */
string SentenceTokenizer::toString() const{
	string debugString = "SentenceTokenizer tokens:\n";
	for(vector<string>::const_iterator it = getTokens().begin(); it != getTokens().end(); ++it){
		debugString += *it + "\n";
	}
	return debugString;
}

/**
 * This function takes a text and tokenizes it into string sentences.
 * It takes into account abbreviations when creating the sentences.
 * @param text The text to be tokenized
 * @return A vector of string sentences.
 */
const vector<string> SentenceTokenizer::tokenize(const string &text){
	string temp;
	vector<string> sents;
	for(string::const_iterator it = text.begin(); it != text.end(); ++it){
		if(*it != '.' && *it != '!' && *it != '?')
			temp.push_back(*it);
		else{						  //is a abbrev
			if(*it == '.' && temp.length() != 0 && isAbbreviation(temp) ){
				//cout << temp << " is an abbreviation!" << endl;
				temp.push_back(*it);
			}
			else{
				temp.push_back(*it);
				sents.push_back(temp);
				temp.clear();
			}
		}

	}

	return sents;
}

