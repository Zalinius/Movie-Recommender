/*
 * WordTokenizer.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#include "WordTokenizer.h"
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

/**
 * The primary constructor for WordTokenizer objects
 * @param text The text to be tokenized into words
 * @param allowCopies Whether or not duplicate tokens should be formed
 */
WordTokenizer::WordTokenizer(const string text, bool allowCopies)
:AbstractTokenizer(text, tokenize(text), allowCopies)
{
	//cout << "Sentence Tokenizer Constructed" << endl;
}

WordTokenizer::~WordTokenizer(){

}

/**
 * The polymorphic function used in AbstractTokenizer's << operator
 * @return The number of tokenized words as well as all the tokenized words, separated by newlines.
 */
string WordTokenizer::toString() const{
	string debugString = "WordTokenizer has ";
	debugString += to_string(getTokens().size());
	debugString += " tokens:\n";
	for(vector<string>::const_iterator it = getTokens().begin(); it != getTokens().end(); ++it)
		debugString += *it + "\n";

	return debugString;
}

/**
 * This function takes a text and tokenizes it into string words.
 * It takes into account abbreviations when creating the words.
 * @param text The text to be tokenized
 * @return A vector of string words.
 */
const vector<string> WordTokenizer::tokenize(const string &text){
	string temp;
	vector<string> tokens;
	for(string::const_iterator it = text.begin(); it != text.end(); ++it){
		if(*it == '.'){
			if(temp.length() != 0 && isAbbreviation(temp)){//is abbreviation
				temp.push_back(*it); //Include the '.' as part of the word
				tokens.push_back(temp);
				temp.clear();
			}
			else{ //is end of sentence
				tokens.push_back(temp); //Push back the word, ignoring the '.'
				temp.clear();
			}
		}
		else if(isValidChar(*it))
			temp.push_back(tolower(*it));
		else
			if(temp.length() != 0){
				tokens.push_back(temp);
				temp.clear();
			}
	}//end loop

	if(temp.length() != 0){
		tokens.push_back(temp);
		temp.clear();
	}
	sort(tokens.begin(), tokens.end());
	return tokens;
}

/**
 * Checks whether a character can be part of a word
 * @param c the character to be checked
 * @return True if allowed, False otherwise.
 */
bool WordTokenizer::isValidChar(char c){
	return (isalnum(c) || c == '\'' || c == '-');
}
