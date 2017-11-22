#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "IndexItem.h"
#include "SentenceTokenizer.h"
#include "Sentence.h"
#include "IndexException.h"
#include <string>

/**
 * A descendant of IndexItem, Document objects have their own SentenceTokenizer used to store their file name and it's Sentence objects (sentences)
 */
using namespace std;

class Document: public IndexItem{

public:
	Document(string fileName, bool allowCopies = true)  throw(IndexException);
	virtual ~Document();
    const string& getFileName() const;
    vector<Sentence>& getSentences();
protected:
	virtual string::size_type size() const override;
private:
    static const string getText(string file) throw(IndexException);
    static const vector<Sentence> makeSentences(const SentenceTokenizer sentenceTokens);

	string fileName;
	SentenceTokenizer sentenceTokens;
	vector<Sentence> sentences;

	virtual const string toString() const override;
};



#endif /* DOCUMENT_H_ */
