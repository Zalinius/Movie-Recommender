
#ifndef STOPWORD_H_
#define STOPWORD_H_

#include "Document.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

/**
 * A Stopword object holds a list of stopwords, and can check if a term in its list or not
 */
class Stopword: public Document{

public:
	Stopword(string filename, bool allowCopies = false)  throw(IndexException);
	virtual ~Stopword();

	bool operator()(string token) const;
protected:
	virtual string::size_type size() const override;
private:
	virtual const string toString() const override;
};




#endif /* STOPWORD_H_ */
