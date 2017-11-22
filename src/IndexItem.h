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
	unsigned int termFrequency(string term) const;

	friend ostream & operator<< (ostream & os, const IndexItem & i);

	virtual string::size_type size() const = 0;
	const string& content() const;

	void setContent(string text, bool allowCopies);

protected:

private:
	WordTokenizer tokens;
	virtual const string toString() const = 0;

};


#endif /* INDEXITEM_H_ */
