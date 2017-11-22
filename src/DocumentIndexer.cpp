#include "DocumentIndexer.h"
#include "Sentence.h"
#include "Stopword.h"
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

/**
 * Adds a movie to a DocumentIndexer, used in Task 4 to build up the movie database, creating the terms from all movie plot summaries in the dictionary
 */
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
	Sentence q(s, true, 0);

	Sentence* q1 = &q;
	shared_ptr<Stopword> stopwords = this->getStopwords();

	DocumentIndexer queryIndex(getFileAmount(), stopwords, true);

	//Need to give queryIndex the same dictionary of Terms, but with its own counts & weights (set all to zero first, then read in the query)
	queryIndex.setDictionary(this->getDictionary());

	q1 >> queryIndex;

	queryIndex.getIndex().resize(getFileAmount());	//Ensure queryIndex and the calling Indexer are the same size
	//queryIndex.normalize();

	vector<double> squery;


	vector<string> qTokens = q.getTokens(); //Which is sorted
	string word = "";
	double h = getIndex().size();

	for(vector<string>::const_iterator it = qTokens.cbegin(); it != qTokens.cend(); ++it){
		if(word != *it && !((*stopwords)(*it))){
			double scoreQ = (1+log(q1->termFrequency(*it)))*log(h);//(unsigned double)it->getDocumentFrequency());
			squery.push_back(scoreQ);
		}
		else{
			//do nothing
		}
		word = *it;
	}

	vector<QueryResult> scores;

	//Repeat the squery procedure for each Document and calculate each Document's score
	for(vector<IndexItem*>::const_iterator it = this->getIndex().begin(); it != this->getIndex().end(); ++it){	//iterate through each document
		vector<double> matchingDocweight, pureDocweight;

		string queryWord = "";
		//calculate matchingDocweight
		for(vector<string>::const_iterator it2 = qTokens.cbegin(); it2 != qTokens.cend(); ++it2){ //Iterate through each token in the query
			if(queryWord != *it2 && !((*stopwords)(*it2))){ // It's a new unique query word and not a stopword
				double scoreD = (1+log((*it)->termFrequency(*it2)))*log(h);//calculate the document's weight for the QUERY word //(unsigned double)it->documentFrequency);
				if (scoreD < 0)
					matchingDocweight.push_back(0);
				else
					matchingDocweight.push_back(scoreD);
				queryWord = *it2;
			}
			else{
				//doNothing
			}
		}

		string docWord = "";
		//calculate pureDocweight
		for(vector<string>::const_iterator it2 = (*it)->getTokens().cbegin(); it2 != (*it)->getTokens().cend(); ++it2){ //Iterate through each token in the document
			if(docWord != *it2 && !((*stopwords)(*it2))){
				double scoreD = (1+log((*it)->termFrequency(*it2)))*log(h);///(unsigned double)it->documentFrequency);
				pureDocweight.push_back(scoreD);
			}
			else{
				//do nothing
			}
			docWord = *it2;
		}
		QueryResult k(*it, computeScore(squery, matchingDocweight, pureDocweight));
		scores.push_back(k);
	}

	//Sort result from highest to lowest
	sortByScore(scores);
	//Set top n+1 results, the best result is the query movie itself and will not be displayed
	vector<QueryResult>* result = new vector<QueryResult>;
	unsigned int count = 0;
	for(vector<QueryResult>::const_iterator it = scores.cbegin(); it != scores.cend() && count != n+1; ++it, ++count){
		result->push_back(*it);
	}

	cout << "Returning " << result->size() << " results!" << endl;

	return *result;

}

/**
 * Compares the plots of the Documents of the calling Indexer to the user's query movie for likeness in token weights
 * m: the user's query movie
 * n: the number of query results (movie reommendations) desired, by default n = 5
 * return: the top n results (movies as a QueryResult vector) which best match the query movie
 */
vector<QueryResult>& DocumentIndexer::movieQuery(IndexItem* m1, unsigned int n){
	Movie* m = dynamic_cast<Movie*>(m1);

	vector<double> squery;

	shared_ptr<Stopword> stopwords = this->getStopwords();

	DocumentIndexer queryIndex(getFileAmount(), stopwords, true);

	queryIndex.setDictionary(this->getDictionary());

	m >> queryIndex;

	queryIndex.getIndex().resize(getFileAmount());	//Ensure queryIndex and the calling Indexer are the same size

	vector<string> qTokens = m->getTokens(); //Which is sorted
	string word = "";
	double h = getIndex().size();

	for(vector<string>::const_iterator it = qTokens.cbegin(); it != qTokens.cend(); ++it){
		if(word != *it && !((*stopwords)(*it))){
			double scoreQ = (1+log(m->termFrequency(*it)))*log(h);//(unsigned double)it->getDocumentFrequency());
			squery.push_back(scoreQ);
		}
		else{
			//do nothing
		}
		word = *it;
	}

	vector<QueryResult> scores;

	//Repeat the squery procedure for each Document and calculate each Document's score
	for(vector<IndexItem*>::const_iterator it = this->getIndex().begin(); it != this->getIndex().end(); ++it){	//iterate through each document
		vector<double> matchingDocweight, pureDocweight;

		string queryWord = "";
		//calculate matchingDocweight
		for(vector<string>::const_iterator it2 = qTokens.cbegin(); it2 != qTokens.cend(); ++it2){ //Iterate through each token in the query
			if(queryWord != *it2 && !((*stopwords)(*it2))){ // It's a new unique query word and not a stopword
				double scoreD = (1+log((*it)->termFrequency(*it2)))*log(h);//calculate the document's weight for the QUERY word //(unsigned double)it->documentFrequency);
				if (scoreD < 0)
					matchingDocweight.push_back(0);
				else
					matchingDocweight.push_back(scoreD);
				queryWord = *it2;
			}
			else{
				//doNothing
			}
		}

		string docWord = "";
		//calculate pureDocweight
		for(vector<string>::const_iterator it2 = (*it)->getTokens().cbegin(); it2 != (*it)->getTokens().cend(); ++it2){ //Iterate through each token in the document
			if(docWord != *it2 && !((*stopwords)(*it2))){
				double scoreD = (1+log((*it)->termFrequency(*it2)))*log(h);///(unsigned double)it->documentFrequency);
				pureDocweight.push_back(scoreD);
			}
			else{
				//do nothing
			}
			docWord = *it2;
		}
		QueryResult k(*it, computeScore(squery, matchingDocweight, pureDocweight));
		scores.push_back(k);
	}

	//Sort result from highest to lowest
	sortByScore(scores);
	//Set top n+1 results, the best result is the query movie itself and will not be displayed
	vector<QueryResult>* result = new vector<QueryResult>;
	unsigned int count = 0;
	for(vector<QueryResult>::const_iterator it = scores.cbegin(); it != scores.cend() && count != n+1; ++it, ++count){
		result->push_back(*it);
	}

	return *result;

}
