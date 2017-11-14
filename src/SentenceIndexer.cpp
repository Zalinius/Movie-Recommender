/*
 * SentenceIndexer.cpp
 *
 */

#include "SentenceIndexer.h"
#include <algorithm>

/**
 * Default constructor, keeps the compiler happy
 */
SentenceIndexer::SentenceIndexer() {}


/**
 * Parameterized constructor, calls the constructor for Indexer with the argument fileAmount
 */
SentenceIndexer::SentenceIndexer(int fileAmount) : Indexer(fileAmount) {

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
	idx.setNormalized(false);

	idx.createTerms(s->getTokens(), idx.getIndex().size()-1);

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
vector<QueryResult>& SentenceIndexer::query(string s, int n){

	try{
		if(getNormalized() == false)
			throw NON_NORMALIZED_INDEX;
	}
	catch (EXCEPTIONS e){
		cout << "Index is not normalized";
		exit(1);
	}


	Indexer::sortDict();


	//Tokenize s and normalize it relative to the Indexer's dictionary to get squery, the query's weight vector
	vector<double> squery;
	Sentence q(s, true, 0);

	Sentence* q1 = &q;

	SentenceIndexer queryIndex(getFileAmount());

	//Need to give queryIndex the same dictionary of Terms, but with its own counts & weights (set all to zero first, then read in the query)
	queryIndex.setDictionary(this->getDictionary());

	for(unsigned int i = 0; i != this->size(); ++i){	//iterate through each document
		for(unsigned int j = 0; j != queryIndex.getDictionary().size(); ++j){  //iterate through each term
			queryIndex.getDictionary().at(j).termFrequencies.at(i) = 0;			//set all term frequencies in the queryIndex to 0, to be overwritten later
			queryIndex.getDictionary().at(j).documentFrequency = 1;
		}
	}

	q1 >> queryIndex;	//Increments the term frequencies in queryIndex's dictionary


	queryIndex.getIndex().resize(getFileAmount());	//Ensure queryIndex and the calling Indexer are the same size
	queryIndex.normalize();


	for(unsigned int i = 0; i != this->getDictionary().size(); ++i){
		squery.push_back(queryIndex.getDictionary().at(i).weight.at(0));		//Fill squery, the query's weight vector
	}


/*
//Debug table
			cout << "\nlibrary dictionary counts" << endl;
			for(unsigned int i = 0; i != this->size(); ++i){	//iterate through each document
					for(unsigned int j = 0; j != this->getDictionary().size(); ++j){  //iterate through each term
						cout << this->getDictionary().at(j).termFrequencies.at(i) << " ";
					}
					cout << endl;
				}

//Debug table
		cout << "\nlibrary dictionary weights (normalized)" << endl;
		for(unsigned int i = 0; i != this->size(); ++i){	//iterate through each document
				for(unsigned int j = 0; j != this->getDictionary().size(); ++j){  //iterate through each term
					cout << this->getDictionary().at(j).weight.at(i) << " ";
				}
				cout << endl;
			}

//Debugging table
		cout << "\nq_i dictionary counts" << endl;
		for(unsigned int i = 0; i != this->size(); ++i){	//iterate through each document
			for(unsigned int j = 0; j != queryIndex.getDictionary().size(); ++j){  //iterate through each term
				cout << queryIndex.getDictionary().at(j).termFrequencies.at(i) << " ";
			}
			cout << endl;
*/

/*
		}

//Debug table
		cout << "\nq_i dictionary weights (normalized)" << endl;
		for(unsigned int i = 0; i != queryIndex.size(); ++i){	//iterate through each document
			for(unsigned int j = 0; j != queryIndex.getDictionary().size(); ++j){  //iterate through each term
				cout << queryIndex.getDictionary().at(j).weight.at(i) << " ";
			}
			cout << endl;
		}*/

		//Repeat the squery procedure for each document and calculate each document's score

		vector<QueryResult> scores;
		for(int i = 0; i != this->size(); ++i){	//iterate through each document
		vector<double> docweight;
		for(int j = 0; j != this->getDictionary().size(); ++j){ //iterate through each term
			docweight.push_back(this->getDictionary()[j].weight[i]);
		}
		QueryResult k(getIndex()[i], computeScore(squery, docweight));
		scores.push_back(k);
	}

	//Sort result from highest to lowest
	sortByScore(scores);

	//Get sentences back, sort by document, push QueryResult sentences onto result and meet the word count

	//Set top n results
	vector<QueryResult>* topSentences = new vector<QueryResult>();
	int wordCount = 0, count = 0;
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


