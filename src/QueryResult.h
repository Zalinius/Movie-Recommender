
#ifndef QUERY_RESULT_H_
#define QUERY_RESULT_H_

#include "IndexItem.h"

/*
 * An object used in the function query(string, int) which returns a vector<Query_Result>, 
 * where each result object has a document and its score.
 */

class QueryResult {

public:

	QueryResult();
	QueryResult(IndexItem* i, double score);
	~QueryResult();


	friend const bool operator< (const QueryResult & left, const QueryResult & right);

	double getScore();
	void setScore(double newScore);
	IndexItem* getI();

private:

	IndexItem *i;
	double score;
};



#endif /* QUERY_RESULT_H_ */
