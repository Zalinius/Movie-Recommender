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
DocumentIndexer::DocumentIndexer(int fileAmount, shared_ptr<Stopword> stopwords, bool omitStopwords) : Indexer(fileAmount, stopwords, omitStopwords) {

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

	shared_ptr<Stopword> stopwords = idx.getStopwords();
	bool omitStopwords = idx.getStopBool();

	idx.createTerms(d->getTokens(), idx.getIndex().size()-1, stopwords, omitStopwords);

	return *d;
}


/**
 * Adds a Sentence to a DocumentIndexer, creating the terms in the dictionary. Used only for the query library, which holds only the query sentence.
 */
IndexItem & operator>> (Sentence *s, DocumentIndexer& idx){
	idx.getIndex().push_back(s);

	shared_ptr<Stopword> stopwords = idx.getStopwords();
	bool omitStopwords = idx.getStopBool();

	idx.createTerms(s->getTokens(), 0, stopwords, omitStopwords);

	return *s;
}

IndexItem & operator>> (Movie *m, DocumentIndexer& idx){
	idx.getIndex().push_back(m);

	shared_ptr<Stopword> stopwords = idx.getStopwords();
	bool omitStopwords = idx.getStopBool();

	idx.createTerms(m->getTokens(), 0, stopwords, omitStopwords);

	return *m;
}


/**
 * Compares the Documents of the calling Indexer to the user's query for likeness in token weights
 * s: the user's query string
 * n: the number of query results desired, by default n = 10
 * return: the top n results (document names and scores) which best match the query
 */
vector<QueryResult>& DocumentIndexer::query(string s, unsigned int n){

	//Tokenize s and normalize it relative to the Indexer's dictionary to get squery, the query's weight vector
	vector<float> squery;
	Sentence q(s, true, 0);

	Sentence* q1 = &q;
	shared_ptr<Stopword> stopwords = this->getStopwords();

	DocumentIndexer queryIndex(getFileAmount(), stopwords, true);

	//Need to give queryIndex the same dictionary of Terms, but with its own counts & weights (set all to zero first, then read in the query)
	queryIndex.setDictionary(this->getDictionary());

	q1 >> queryIndex;

	queryIndex.getIndex().resize(getFileAmount());	//Ensure queryIndex and the calling Indexer are the same size
	//queryIndex.normalize();

	double h = getIndex().size();
	for(set<Term>::const_iterator it = queryIndex.getDictionary().begin(); it != queryIndex.getDictionary().end(); ++it){	//iterate through all terms
			//squery.push_back(queryIndex.getDictionary().at(i).weight.at(0));
			if (q1->termFrequency(it->getWord()) == 0)
				squery.push_back(0);
			else{
				float score = (1+log(q1->termFrequency(it->getWord())))*log(h);///(float)it->documentFrequency);
				squery.push_back(score);
			}
			//Fill squery, the query's weight vector
	}
	//Repeat the squery procedure for each Document and calculate each Document's score
	vector<QueryResult> scores;
	for(vector<IndexItem*>::const_iterator it2 = this->getIndex().begin(); it2 != this->getIndex().end(); ++it2){	//iterate through each document
		vector<float> docweight;
		for(set<Term>::const_iterator it = this->getDictionary().begin(); it != this->getDictionary().end(); ++it){ //iterate through each term
			//docweight.push_back(this->getDictionary()[j].weight[i]);
			if ((*it2)->termFrequency(it->getWord()) == 0)
				docweight.push_back(0);
			else
			{
				float score = (1+log((*it2)->termFrequency(it->getWord())))*log(h/(float)it->getDocumentFrequency());
				docweight.push_back(score);
			}
		}
		QueryResult k(*it2, computeScore(squery, docweight));
		scores.push_back(k);
	}

	//Sort result from highest to lowest
	sortByScore(scores);

	//Set top n results
	vector<QueryResult>* result = new vector<QueryResult>;
	for(int i = 0; i != n; ++i){
		result->push_back(scores[i]);
	}

	return *result;

}


vector<QueryResult>& DocumentIndexer::movieQuery(IndexItem* m1, unsigned int n){
	Movie* m = dynamic_cast<Movie*>(m1);

	vector<float> squery;

	shared_ptr<Stopword> stopwords = this->getStopwords();

	DocumentIndexer queryIndex(getFileAmount(), stopwords, true);

	queryIndex.setDictionary(this->getDictionary());

	m >> queryIndex;

	queryIndex.getIndex().resize(getFileAmount());	//Ensure queryIndex and the calling Indexer are the same size

	cout << this->getDictionary().size() << " unique words" << endl;
	double h = getIndex().size();
	for(set<Term>::const_iterator it = queryIndex.getDictionary().begin(); it != queryIndex.getDictionary().end(); ++it){	//iterate through all terms
			//squery.push_back(queryIndex.getDictionary().at(i).weight.at(0));
			if (m->termFrequency(it->getWord()) == 0)
				squery.push_back(0);
			else{
				float score = (1+log(m->termFrequency(it->getWord())))*log(h);///(float)it->documentFrequency);
				squery.push_back(score);
			}
			//Fill squery, the query's weight vector
	}

	//Repeat the squery procedure for each Document and calculate each Document's score
	vector<QueryResult> scores;
	for(vector<IndexItem*>::const_iterator it2 = this->getIndex().begin(); it2 != this->getIndex().end(); ++it2){	//iterate through each document
		vector<float> docweight;
		for(set<Term>::const_iterator it = this->getDictionary().begin(); it != this->getDictionary().end(); ++it){ //iterate through each term
			//docweight.push_back(this->getDictionary()[j].weight[i]);
			unsigned int termFreq = (*it2)->termFrequency(it->getWord());
			if (termFreq == 0)
				docweight.push_back(0);
			else
			{
				float score = (1+log(termFreq))*log(h/(float)it->getDocumentFrequency());
				docweight.push_back(score);
			}
		}
		QueryResult k(*it2, computeScore(squery, docweight));
		scores.push_back(k);
		if (scores.size()%100 == 0)
			cout << scores.size() << " movie scores calculated" << endl;
	}

	//Sort result from highest to lowest
	sortByScore(scores);


	//Set top n results
	vector<QueryResult>* result = new vector<QueryResult>;
	for(unsigned int i = 0; i != n; ++i){
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
