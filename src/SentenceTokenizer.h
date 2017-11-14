/*
 * SentenceTokenizer.h
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#ifndef SENTENCETOKENIZER_H_
#define SENTENCETOKENIZER_H_

#include "AbstractTokenizer.h"
#include <string>

class Sentence;

using namespace std;

class SentenceTokenizer: public AbstractTokenizer{
public:
	SentenceTokenizer(const string text, bool allowCopies = true);
	virtual ~SentenceTokenizer();
protected:
private:
	virtual string toString() const;
	virtual const vector<string> tokenize(const string &text);

	};




#endif /* SENTENCETOKENIZER_H_ */
