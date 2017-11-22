#include "Indexer.h"
#include "Stopword.h"
#include "Term.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;


/**
 * Parameterized constructor, takes a specified size for the parameter fileAmount
 */
Indexer::Indexer(int fileAmount, shared_ptr<Stopword> stopwords, bool omitStopwords) :  dictionary(set<Term>()), index(vector<IndexItem*>()), fileAmount(fileAmount), stopwords(stopwords), omitStopwords(omitStopwords){}

/**
 * Destructor
 */
Indexer::~Indexer() {
}


/** Used for debugging, calls toString() on the supplied Rvalue to allow for polymorphism
 * @param os the chosen output stream fpr debugging
 * @return the modified output string
 */
ostream & Indexer::operator<< (ostream & os){
	return os << toString();
}


/**
 * @param i the index to be accessed
 * @return a const pointer to an IndexItem
 */
const IndexItem* Indexer::operator[](int i){
	return index[i];
}

/**
 * @param i the index to be accessed
 * @return a pointer to an IndexItem
 */
IndexItem* Indexer::operator[](int i) const{
	return index[i];
}

/**
 * @return a string summarizing the Indexer object, used primarily for debugging
 */
string Indexer::toString() const {
	string s1 = "Index size: " + size();
	return s1;
}

/**
 * @return the int fileAmount
 */
unsigned int Indexer::getFileAmount(){
	return fileAmount;
}


shared_ptr<Stopword> Indexer::getStopwords(){
	return stopwords;
}

bool Indexer::getStopBool(){
	return omitStopwords;
}

/**
 * @return the vector<IndexItem*> index
 */
vector<IndexItem*>& Indexer::getIndex(){
	return index;
}

/**
 * @return the vector<Term> dictionary
 */
set<Term>& Indexer::getDictionary(){
	return dictionary;
}

/** Gives an indexer a new dictionary, used in querying to match the query dictionary to the document library
 * @param newDictionary the vector<Term> value that the dictionary is being set to
 */
void Indexer::setDictionary(set<Term> newDictionary){
	dictionary = newDictionary;
}

/*
 * @return the number of documents (fileAmount) stored in the calling Indexer object
 */
unsigned int Indexer::size() const{
	return fileAmount;
}

/**
 * Helper function used in query(), sorts the results of the document scores from greatest to least
 * scores: a vector of QueryResult objects to be sorted
 * return: sorted version of scores vector
 */
void Indexer::sortByScore(vector<QueryResult> & scores) {
	sort(scores.begin(), scores.end());
	reverse(scores.begin(), scores.end());
}

/*
 * A helper function used by both derived class' query() functions. Computes the cosine similarity between the document vector docweight and
 * the query vector squery according to the formula provided in the assignment instructions
 * @param squery the normalized weight vector of the user's query
 * @paramdocweight the normalized weight vector for the document being compared to the query
 * @return the score of a document's weight vector in comparison to the query's weight vector
 */
double Indexer::computeScore(vector<double> squery, vector<double> matchingDocweight, vector<double> pureDocweight){

	double numsum = 0, densum1 = 0, densum2 = 0;

	vector<double>::const_iterator sqIt, mDwIt;
	for(sqIt = squery.cbegin(), mDwIt = matchingDocweight.cbegin(); sqIt != squery.cend() && mDwIt != matchingDocweight.cend(); ++sqIt, ++mDwIt){
		numsum += (*sqIt)*(*mDwIt);
		densum1 += (*sqIt)*(*sqIt);
	}

	vector<double>::const_iterator pDwIt;
	for(pDwIt = pureDocweight.cbegin(); pDwIt != pureDocweight.cend(); ++pDwIt){
		densum2 += (*pDwIt)*(*pDwIt);
	}

	double den = sqrt(densum1)*sqrt(densum2);
	if (den < 0){
		cout << "Divide by zero, fatal error in query" << endl;
		exit(1);
	}
	if (den == 0)
		return 0;
	return numsum/den;
}

/** Creates Term objects and stores them in the calling object's dictionary
 * @param tokens a vector of the tokens to be added to the dictionary
 * @param docNo the number of the document the term came from
 */
void Indexer::createTerms(vector<string> tokens, int docNo, shared_ptr<Stopword> stopwords, bool omitStopwords) {
	string currentToken = "";
	for(vector<string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it){

		Term newTerm(*it);
		if (currentToken == newTerm.getWord() || (omitStopwords && (*stopwords)(*it)))
			continue;

		pair<set<Term>::iterator, bool> info = dictionary.insert(newTerm);
		currentToken = newTerm.getWord();
		if (!info.second)	//the term already exists in the dictionary,
			info.first->incrementDocumentFrequency();
	}
}

/*
 * Normalizes an Indexer object with relative weights for each term. The Indexer's value normalized is set to true after this procedure
 * tokens: a vector of Terms which the Indexer is normalized relative to
 *
 * The normalized weights are calculated as follows:
 * n is the total number of documents in the Indexer object's index vector
 * tf(t,d) is the word count for a term t (from the tokens dictionary)found in document d (a document in the Indexer object's index vector)
 * dft is the document frequency of t, namely the number of documents t is found in
 * These values are used to calculate the weight of a term t in a document d according to the formula:
 * w(t,d) = (1 + log(tf(t,d))) * log(N/dft)
// */
//void Indexer::normalize(){
//	double n = getIndex().size();	//Document count: n is defined as the total number of documents in your index
//	for(int i = 0; i != dictionary.size(); ++i) {	//iterate through all terms in the dictionary
//		for(int j = 0; j != n; ++j) {	//iterate through all documents in the indexer calling object
//			if (dictionary[i].termFrequencies[j] == 0)
//				dictionary.at(i).weight.at(j) = 0;
//			else
//				dictionary.at(i).weight.at(j) = ((1+log(dictionary[i].termFrequencies[j]))*log(n/(double)dictionary[i].documentFrequency));
//		}
//	}
//	setNormalized(true);
//}




