#include "Stopword.h"
#include "Document.h"
#include "WordTokenizer.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;


/**
 * The primary Stopword constructor, reads and tokenizes the stopword list from the argument filename
 * @param filename The name of the Stopword file
 * @param allowCopies Whether or not duplicate stopword tokens should be allowed. Generally this is false.
 */
Stopword::Stopword(string filename, bool allowCopies)  throw(IndexException)
:Document(filename, allowCopies)
{
	//cout << "Stopword Constructed" << endl;

}

/**
 * Destructor
 */
Stopword::~Stopword(){
}



string::size_type Stopword::size() const{
	return getTokens().size();
}


/**
 * Overloaded operator(), finds the argument token in the list of stopwords in the Stopword object
 * return: true if the token is found, false otherwise
 */
bool Stopword::operator()(string token) const{
	std::transform(token.begin(), token.end(), token.begin(), ::tolower);
	vector<string>::const_iterator result = lower_bound(getTokens().cbegin(), getTokens().cend(), token);
	return result != getTokens().cend();
}

/**
 * The polymorphic function used in IndexItem's << operator
 * @return The original file's name, the number of stopwords.
 */
const string Stopword::toString() const{
	string debugString;
	debugString = "File: " + getFileName() + " has ";
	debugString += to_string(getTokens().size());
	debugString += " stopwords\n";
	return debugString;
}
