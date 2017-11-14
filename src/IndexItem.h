/*
 * IndexItem.h
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#ifndef INDEXITEM_H_
#define INDEXITEM_H_

#include "WordTokenizer.h"
#include <string>
#include <vector>

using namespace std;

class IndexItem{
public:
	IndexItem(const string text, bool allowCopies);
	virtual ~IndexItem();
	const vector<string>& getTokens() const;

	friend ostream & operator<< (ostream & os, const IndexItem & i);

	virtual string::size_type size() const = 0;
	const string& content() const;

protected:

private:
	WordTokenizer tokens;
	virtual const string toString() const = 0;

};


#endif /* INDEXITEM_H_ */
