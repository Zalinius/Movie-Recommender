/*
 * Sentence.h
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#ifndef SENTENCE_H_
#define SENTENCE_H_

#include "IndexItem.h"
#include <string>

using namespace std;


class Sentence: public IndexItem{
public:
	Sentence(const string &sentence, bool allowCopies = true, unsigned int pos = 0);
	virtual ~Sentence();

	virtual string::size_type size() const override;
	void setDoc(unsigned int docNumber);

	friend bool operator< (const Sentence & left, const Sentence & right);


protected:
	unsigned int getPos() const;

private:
	unsigned int pos;
	unsigned int doc;
	virtual const string toString() const override;
};


#endif /* SENTENCE_H_ */
