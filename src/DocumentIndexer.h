/*
 * DocumentIndexer.h
 *
 *  Created on: Nov 5, 2017
 *      Author: Zachary
 */

#ifndef SRC_DOCUMENTINDEXER_H_
#define SRC_DOCUMENTINDEXER_H_


#include "Indexer.h"
#include "Document.h"
#include "Movie.h"
#include <vector>
#include <iostream>


/**
 * The DocumentIndexer works as defined in Assignment #2, indexing and querying complete documents with a user's query.
 */
class DocumentIndexer : public Indexer {

public:

	DocumentIndexer(int fileAmount);
	virtual ~DocumentIndexer();

	vector<QueryResult>& query(string s, int n = 10);

	friend IndexItem & operator>> (Document *d, DocumentIndexer& idx);
	friend IndexItem & operator>> (Sentence *s, DocumentIndexer& idx);

	void addMovie(Movie *m, shared_ptr<Stopword> stopwords);


protected:

	virtual string toString() const;


};

#endif /* SRC_DOCUMENTINDEXER_H_ */
