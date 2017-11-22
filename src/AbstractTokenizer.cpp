#include "AbstractTokenizer.h"
#include "Stopword.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

Stopword AbstractTokenizer::abbreviations("ABBREVIATIONS.txt");


/**
 * The constructor for AbstractTokenizer.
 * As it is abstract, it is mainly used to initialize common private member of derived classes.
 * @param text A constant reference to the text to be tokenized.
 * @param allowCopies Whether or not duplicate tokens will be kept during construction.
 */
AbstractTokenizer::AbstractTokenizer(const string text, const vector<string> words, bool allowCopies)
:text(text), words(words) , allowCopies(allowCopies)
{
	//cout << "Abstract Tokenizer Constructed" << endl;
}

AbstractTokenizer::~AbstractTokenizer(){
	//cout << "AbstractTokenizer Destroyed" << endl;
}

/**
 * A simple getter for the stored text
 * @return the text tokenized by the tokenizer
 */
const string& AbstractTokenizer::getText() const {
	return text;
}

/**
 * @return Returns a vector of the *word* tokens, by const reference.
 */
const vector<string>& AbstractTokenizer::getTokens() const{
	return words; //Returns the tokens by const reference
}

/**
 * This insertion operator is used for AbstractTokenizer as well as all it's derived classes.
 * It is "polymorphic" as it relies on a virtual method toString to operate.
 */
ostream & operator<< (ostream & os, AbstractTokenizer & t){
	os << t.toString();
	return os;
}

/**
 * @term an unpunctuated, whitespace free string
 * @return Whether or not a term is an abbreviation.
 */
bool AbstractTokenizer::isAbbreviation(string term){
						//is abbreviation   is single letter abbreviation
	return (abbreviations(lastWord(term)) || (lastWord(term).size() == 1 && isalpha(lastWord(term)[0])) );
}

/**
 * @param A string of words.
 * @return The final word of a string of words.
 */
string AbstractTokenizer::lastWord(const string& temp){
	string word;
	for(string::const_reverse_iterator it = temp.rbegin(); it != temp.rend(); ++it){
		if(isalnum(*it) || *it == '\'' || *it == '-' ){
			word = *it + word;
		}
		else{
			return word;
		}
	}
	return word;
}
