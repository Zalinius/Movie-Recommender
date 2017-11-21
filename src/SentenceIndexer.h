/*
 * SentenceIndexer.h
 */

#ifndef SRC_SENTENCEINDEXER_H_
#define SRC_SENTENCEINDEXER_H_

#include <vector>
#include <iostream>
#include "QueryResult.h"
#include "Sentence.h"
#include "Indexer.h"

/**
 * . The SentenceIndexer receives tokenized documents (vector of sentences, each of which has a vector of words), uses this to build the index of sentences
 * and the dictionary of terms, then queries the resulting sentence-term matrix (sentences and their scores to the query question) to generate the essay.
 */
class SentenceIndexer : public Indexer {

public:
	SentenceIndexer(int fileAmount, shared_ptr<Stopword> stopwords, bool omitStopwords);
	virtual ~SentenceIndexer();

	virtual string toString() const;

	friend IndexItem & operator>> (Sentence *s, SentenceIndexer& idx);

	vector<QueryResult>& query(string s, unsigned int n = 500);

private:

	static bool compareQuery(QueryResult q1, QueryResult q2);
};



#endif /* SRC_SENTENCEINDEXER_H_ */
