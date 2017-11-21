#ifndef INDEXER_H_
#define INDEXER_H_

#include "IndexItem.h"
#include "Term.h"
#include "QueryResult.h"
#include <memory>
#include <set>
#include <iostream>

enum EXCEPTIONS {NON_NORMALIZED_INDEX};

using namespace std;

/**
 * The Indexer class is a base for DocumentIndexer and SentenceIndexer, and their shared functions, such as normalizing and querying,
 * are defined in the implementation file
 */
class Indexer {

public:
	Indexer();
	Indexer(int fileAmount);
	virtual ~Indexer();

	virtual string toString() const;

	const IndexItem* operator[](int i);
	IndexItem* operator[](int i) const;

	ostream & operator<< (ostream & os);

	//void normalize();
	vector<IndexItem*>& getIndex();
	void sortDict();
	set<Term>& getDictionary();

protected:

	virtual vector<QueryResult>& query(string s, int n) = 0;
	unsigned int size() const;
	//bool getNormalized() const;
	//void setNormalized(bool facts);
	unsigned int getFileAmount();
	void setDictionary(set<Term> dictionary);



	virtual void createTerms(vector<string>, int docNo, shared_ptr<Stopword> stopwords, bool omitStopwords);

	static double computeScore(vector<float> squery, vector<float> docweight);
	static void sortByScore(vector<QueryResult> & scores);



private:

	set<Term> dictionary;
	vector<IndexItem*> index;

	unsigned int fileAmount;
	//bool normalized;


};


#endif /* INDEXER_H_ */
