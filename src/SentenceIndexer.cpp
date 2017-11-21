/*
 * SentenceIndexer.cpp
 *
 */

#include "SentenceIndexer.h"
#include <algorithm>
#include <math.h>


/**
 * Parameterized constructor, calls the constructor for Indexer with the argument fileAmount
 */
SentenceIndexer::SentenceIndexer(int fileAmount, shared_ptr<Stopword> stopwords, bool omitStopwords) : Indexer(fileAmount, stopwords, omitStopwords) {

}


/**
 * Destructor
 */
SentenceIndexer::~SentenceIndexer() {
}


/**
 * @return a string summarizing the ScentenceIndexer object, used primarily for debugging
 */
string SentenceIndexer::toString() const{
	string s1 = "Indexer type: Sentence ";
	s1 += this->Indexer::toString();
	return s1;
}


/**
 * Adds a Sentence to a SentenceIndexer, creating the terms in the dictionary and filling the index with Sentence*
 */
IndexItem & operator>> (Sentence *s, SentenceIndexer& idx){
	idx.getIndex().push_back(s);
	//idx.setNormalized(false);

	shared_ptr<Stopword> stopwords = idx.getStopwords();
	bool omitStopwords = idx.getStopBool();

	idx.createTerms(s->getTokens(), idx.getIndex().size()-1, stopwords, omitStopwords);

	return *s;
}




//Override the query function for the sentence indexer, so that the second int
//argument now defines the maximum total words in the returned sentences (e.g., when you set it to 500
//words, it will return as many top-ranked sentences as possible to fit in the total length of 500 words)
//string s is read in from the question file
/** Finds the top matching sentences to the query within the word count limit
 * @param s a string of the essay question
 * @param n the word count limit for the essay being generated
 * @return a vector<QueryResult> of the top matching sentences in a coherent order
 */
vector<QueryResult>& SentenceIndexer::query(string s, unsigned int n){

//	try{
//		if(getNormalized() == false)
//			throw NON_NORMALIZED_INDEX;
//	}
//	catch (EXCEPTIONS e){
//		cout << "Index is not normalized";
//		exit(1);
//	}


	//Tokenize s and normalize it relative to the Indexer's dictionary to get squery, the query's weight vector
	vector<float> squery;
	Sentence q(s, true, 0);

	Sentence* q1 = &q;
	shared_ptr<Stopword> stopwords = this->getStopwords();
	SentenceIndexer queryIndex(getFileAmount(), stopwords, false);

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
			if (q1->termFrequency(it->getWord()) == 0)
				squery.push_back(0);
			else{
				float score = (1+log(q1->termFrequency(it->getWord())))*log(h/(float)it->getDocumentFrequency());
				cout << *it << " score: " << score << endl;
				squery.push_back(score);
			}
			//Fill squery, the query's weight vector
	}

		//Repeat the squery procedure for each document and calculate each document's score

		vector<QueryResult> scores;
		for(unsigned int i = 0; i != this->size(); ++i){	//iterate through each document
			vector<float> docweight;
			for(set<Term>::const_iterator it = queryIndex.getDictionary().begin(); it != queryIndex.getDictionary().end(); ++it){ //iterate through each term
				//docweight.push_back(this->getDictionary()[j].weight[i]);
				if (q1->termFrequency(it->getWord()) == 0)
					docweight.push_back(0);
				else
				{
					float score = (1+log(q1->termFrequency(it->getWord())))*log(h/(float)it->getDocumentFrequency());
					docweight.push_back(score);
				}
			}
			QueryResult k(getIndex()[i], computeScore(squery, docweight));
			scores.push_back(k);
		}

	//Sort result from highest to lowest
	sortByScore(scores);

	//Get sentences back, sort by document, push QueryResult sentences onto result and meet the word count

	//Set top n results
	vector<QueryResult>* topSentences = new vector<QueryResult>();
	int wordCount = 0;
	unsigned int count = 0;
	do {
		wordCount += scores[count].getI()->size();

		if (wordCount <= n)
		{
			topSentences->push_back(scores[count]);
			count++;
		}
	}while (wordCount <= n && count < scores.size());

	sort(topSentences->begin(), topSentences->end(), compareQuery);

	return *topSentences;
}


/**
 * Free function used to compare QueryResults by their document number and position in the document
 * @param q1 the left QueryResult object being compared
 * @param q2 the right QueryResult object being compared
 * @return true if q1 object's score is less than q2's
 */
bool SentenceIndexer::compareQuery(QueryResult q1, QueryResult q2){
	Sentence s1 = *dynamic_cast<Sentence*>(q1.getI());
	Sentence s2 = *dynamic_cast<Sentence*>(q2.getI());
	return s1 < s2;
}


