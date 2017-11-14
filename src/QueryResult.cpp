/*
 * Query_result.cpp
 *
 *  Created on: Oct 16, 2017
 *      Author: n_parme
 */

#include "QueryResult.h"

#include "IndexItem.h"

using namespace std;

/**
* Default constructor, points to a newly made default IndexItem and sets score to 0
*/
QueryResult::QueryResult() : i(), score(0) {}


/**
* Parameterized constructor, takes an IndexItem and a score, uses these arguments to initialize the corresponding parameters
*/
QueryResult::QueryResult(IndexItem* i, double score)
: i(i), score(score){}


/**
* Destructor
*/
QueryResult::~QueryResult(){}


/**
 * The < operator, redefined to sort QueryResult objects by their score
 * @param left the left QueryResult object being compared
 * @param right the right QueryResult object being compared
 * @return true if the left object's score is less than the right's
 */
const bool operator< (const QueryResult & left, const QueryResult & right){
	return left.score < right.score;
}


/**
 * @return the variable score
 */
double QueryResult::getScore() {
	return score;
}


/**
 * A setter for a QueryResult's score
 * @param newScore the QueryResult's new score
 */
void QueryResult::setScore(double newScore) {
	score = newScore;
}

/**
 * @return a pointer to the vector<IndexItem*> called index
 */
IndexItem* QueryResult::getI(){
	return i;
}
