/*
 * WordTokenizer.h
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#ifndef WORDTOKENIZER_H_
#define WORDTOKENIZER_H_

#include "AbstractTokenizer.h"
#include <string>

using namespace std;

class WordTokenizer : public AbstractTokenizer{

public:
	WordTokenizer(const string text, bool allowCopies = true);
	virtual ~WordTokenizer() override;

protected:
private:
	virtual string toString() const override;
	virtual const vector<string> tokenize(const string &text) override;

	static bool isValidChar(char c);
};




#endif /* WORDTOKENIZER_H_ */
