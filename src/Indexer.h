#ifndef INDEXER_H_
#define INDEXER_H_

#include "IndexItem.h"
#include "Term.h"
#include "QueryResult.h"
#include <memory>
#include <vector>
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

	void normalize();
	vector<IndexItem*>& getIndex();
	void sortDict();
	vector<Term>& getDictionary();

protected:

	virtual vector<QueryResult>& query(string s, int n) = 0;
	unsigned int size() const;
	bool getNormalized() const;
	void setNormalized(bool facts);
	unsigned int getFileAmount();
	void setDictionary(vector<Term> dictionary);



	virtual void createTerms(vector<string>, int docNo);
	void createTerms(vector<string> tokens, int docNo, shared_ptr<Stopword> stopwords);

	static double computeScore(vector<double> squery, vector<double> docweight);
	static void sortByScore(vector<QueryResult> & scores);



private:

	vector<Term> dictionary;
	vector<IndexItem*> index;

	unsigned int fileAmount;
	bool normalized;


};


#endif /* INDEXER_H_ */
