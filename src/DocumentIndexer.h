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

	DocumentIndexer(int fileAmount, shared_ptr<Stopword> stopwords, bool omitStopwords);
	virtual ~DocumentIndexer();

	vector<QueryResult>& query(string s, unsigned int n = 10);
	vector<QueryResult>& movieQuery(IndexItem* m, unsigned int n = 5);

	friend IndexItem & operator>> (Document *d, DocumentIndexer& idx);
	friend IndexItem & operator>> (Sentence *s, DocumentIndexer& idx);
	friend IndexItem & operator>> (Movie *m, DocumentIndexer& idx);


protected:

	virtual string toString() const;


};

#endif /* SRC_DOCUMENTINDEXER_H_ */
