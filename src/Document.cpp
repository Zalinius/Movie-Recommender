#include "Document.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

using namespace std;

/**
 * The primary constructor for the class Document.
 * @param fileName The name of the file to be read in.
 * @allowCopies whether or not copies should be kept when tokenizing the text.
 */
Document::Document(string fileName, bool allowCopies)  throw(IndexException)
:IndexItem(getText(fileName), allowCopies), fileName(fileName), sentenceTokens(SentenceTokenizer(content(), allowCopies)), sentences(makeSentences(sentenceTokens))
{
	//cout << "Document Constructed" << endl;
}

Document::~Document(){
}

/**
 * A getter function for the size of the document in chars.
 * @return The number of characters in the document.
 */
string::size_type Document::size() const{
	return (sentenceTokens.getText()).size();
}

/**
 * @return The name of the file used to construct this Document object.
 */
const string& Document::getFileName() const{
	return fileName;
}

/**
 * @return A vector of the sentences of the Document object.
 */
vector<Sentence>& Document::getSentences(){
	return sentences;
}

/**
 * This static helper function loads in the contents of a text file, and returns the text.
 * @param file The name of the file.
 * @return A single string containing the entire text.
 */
const string Document::getText(string file) throw(IndexException){
	ifstream docIn;

	docIn = ifstream(file.c_str());
	if(!docIn)
		throw IndexException(file);

	string text;
	char temp;
	while(docIn >> noskipws >> temp){
		text += temp;
	}
	docIn.close();
	return text;
}

/**
 * This static helper method creates a vector of Sentences from a SentenceTokenizer's vector of strings.
 * @param A pointer to the tokenized sentence strings.
 * @return A vector of Sentence objects
 */
const vector<Sentence> Document::makeSentences(const SentenceTokenizer sentenceTokens){
	vector<Sentence> newSentences;

	const vector<string> tokens = sentenceTokens.getTokens();
	unsigned int pos = 0;
	for(vector<string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it){
		++pos;
		//cout << "Sentence is: " << *it << endl;
		newSentences.push_back(Sentence(*it, true,pos));
	}

	return newSentences;
}

/**
 * The polymorphic function used in IndexItem's << operator
 * @return The original file's name, the number of sentences and the text.
 */
const string Document::toString() const{
	string debugString;
	debugString += "File: " + fileName + " contains " + to_string(sentences.size()) + " sentences." + "\nText: " + content() + "\n";
	return debugString;
}
