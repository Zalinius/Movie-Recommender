/*
 * Document.h
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "IndexItem.h"
#include "SentenceTokenizer.h"
#include "Sentence.h"
#include <string>


using namespace std;

class Document: public IndexItem{

public:
	Document(string fileName, bool allowCopies = true);
	virtual ~Document();
    const string& getFileName() const;
    vector<Sentence>& getSentences();
protected:
	virtual string::size_type size() const override;
private:
    static const string getText(string file);
    static const vector<Sentence> makeSentences(const SentenceTokenizer sentenceTokens);

	string fileName;
	SentenceTokenizer sentenceTokens;
	vector<Sentence> sentences;

	virtual const string toString() const override;
};



#endif /* DOCUMENT_H_ */
