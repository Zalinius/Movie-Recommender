/*
 * DocumentIndexer.cpp
 *
 *  Created on: Nov 5, 2017
 *      Author: Zachary
 */

#include "DocumentIndexer.h"
#include "Sentence.h"
#include <math.h>
#include <algorithm>

using namespace std;

/*
 * Parameterized constructor, initializes an empty vector of Documents, an empty vector of Terms,
 * and sets the Indexer's normalized value to false to prevent meaningless queries
 */
DocumentIndexer::DocumentIndexer(int fileAmount) : Indexer(fileAmount) {

}


/**
 * Destructor
 */
DocumentIndexer::~DocumentIndexer() {

}

/**
 * @return a string summarizing the DocumentIndexer object, used primarily for debugging
 */
string DocumentIndexer::toString() const{
	string s1 = "Indexer type: Document ";
	s1 += this->Indexer::toString();
	return s1;
}


/**
 * Adds a Document to a DocumentIndexer, creating the terms in the dictionary and filling the index with Document*
 */
IndexItem & operator>> (Document *d, DocumentIndexer& idx){
	idx.getIndex().push_back(d);

	shared_ptr<Stopword> stopwords;

	idx.createTerms(d->getTokens(), idx.getIndex().size()-1, stopwords, false);

	return *d;
}


/**
 * Adds a Sentence to a DocumentIndexer, creating the terms in the dictionary. Used only for the query library, which holds only the query sentence.
 */
IndexItem & operator>> (Sentence *s, DocumentIndexer& idx){
	idx.getIndex().push_back(s);

	shared_ptr<Stopword> stopwords;

	idx.createTerms(s->getTokens(), 0, stopwords, true);

	return *s;
}

void DocumentIndexer::addMovie(Movie *m, shared_ptr<Stopword> stopwords){
	getIndex().push_back(m);
	//setNormalized(false);

	createTerms(m->getTokens(), 0, stopwords, true);
}


/**
 * Compares the Documents of the calling Indexer to the user's query for likeness in token weights
 * s: the user's query string
 * n: the number of query results desired, by default n = 10
 * return: the top n results (document names and scores) which best match the query
 */
vector<QueryResult>& DocumentIndexer::query(string s, int n){

	//Tokenize s and normalize it relative to the Indexer's dictionary to get squery, the query's weight vector
	vector<float> squery;
	Sentence q(s, true, 0);

	Sentence* q1 = &q;

	DocumentIndexer queryIndex(getFileAmount());

	//Need to give queryIndex the same dictionary of Terms, but with its own counts & weights (set all to zero first, then read in the query)
	queryIndex.setDictionary(this->getDictionary());

//	for(unsigned int i = 0; i != this->size(); ++i){	//iterate through each document
//		for(unsigned int j = 0; j != queryIndex.getDictionary().size(); ++j){  //iterate through each term
//			queryIndex.getDictionary().at(j).termFrequencies.at(i) = 0;			//set all term frequencies in the queryIndex to 0, to be overwritten later
//			queryIndex.getDictionary().at(j).documentFrequency = 1;
//		}
//	}

	q1 >> queryIndex;	//Increments the term frequencies in queryIndex's dictionary



	queryIndex.getIndex().resize(getFileAmount());	//Ensure queryIndex and the calling Indexer are the same size
	//queryIndex.normalize();

	double h = getIndex().size();
	for(set<Term>::const_iterator it = queryIndex.getDictionary().begin(); it != queryIndex.getDictionary().end(); ++it){	//iterate through all terms
			//squery.push_back(queryIndex.getDictionary().at(i).weight.at(0));
			if (q1->termFrequency(it->term) == 0)
				squery.push_back(0);
			else{
				float score = (1+log(q1->termFrequency(it->term)))*log(h/(float)it->documentFrequency);
				cout << *it << " score: " << score << endl;
				squery.push_back(score);
			}
			//Fill squery, the query's weight vector
	}
	//Repeat the squery procedure for each Document and calculate each Document's score

	vector<QueryResult> scores;
	for(unsigned int i = 0; i != this->size(); ++i){	//iterate through each document
		vector<float> docweight;
		for(set<Term>::const_iterator it = queryIndex.getDictionary().begin(); it != queryIndex.getDictionary().end(); ++it){ //iterate through each term
			//docweight.push_back(this->getDictionary()[j].weight[i]);
			if (q1->termFrequency(it->term) == 0)
				docweight.push_back(0);
			else
			{
				float score = (1+log(q1->termFrequency(it->term)))*log(h/(float)it->documentFrequency);
				docweight.push_back(score);
			}
		}
		QueryResult k(getIndex()[i], computeScore(squery, docweight));
		scores.push_back(k);
	}

	//Sort result from highest to lowest
	sortByScore(scores);

	cout << "Sorted scores vector" << endl;

	//Set top n results
	vector<QueryResult>* result = new vector<QueryResult>;
	for(int i = 0; i != n; ++i){
		result->push_back(scores[i]);
	}

	return *result;

}


/*
 	//Debug table
		cout << "\nlibrary dictionary weights (hopefully normalized)" << endl;
		for(unsigned int i = 0; i != library->size(); ++i){	//iterate through each document
				for(unsigned int j = 0; j != library->dictionary.size(); ++j){  //iterate through each term
					cout << library->dictionary.at(j).weight.at(i) << " ";
				}
				cout << endl;
			}
	//Debug table
			cout << "\nlibrary dictionary counts" << endl;
			for(unsigned int i = 0; i != library->size(); ++i){	//iterate through each document
					for(unsigned int j = 0; j != library->dictionary.size(); ++j){  //iterate through each term
						cout << library->dictionary.at(j).termFrequencies.at(i) << " ";
					}
					cout << endl;
				}

	//Debug table
		cout << "\nlibrary dictionary weights (not normalized)" << endl;
		for(unsigned int i = 0; i != library->size(); ++i){	//iterate through each document
				for(unsigned int j = 0; j != library->dictionary.size(); ++j){  //iterate through each term
					cout << library->dictionary.at(j).weight.at(i) << " ";
				}
				cout << endl;
			}

//Debugging table
		cout << "\nq_i dictionary counts" << endl;
		for(unsigned int i = 0; i != this->size(); ++i){	//iterate through each document
				for(unsigned int j = 0; j != queryIndex.dictionary.size(); ++j){  //iterate through each term
					cout << queryIndex.dictionary.at(j).termFrequencies.at(i) << " ";
				}
				cout << endl;
			}
*/
