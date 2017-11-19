/*
 * IndexItem.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: lapoi
 */

#include "IndexItem.h"
#include <string>
#include <iostream>
#include <typeinfo>



using namespace std;

/**
 * The primary constructor for the IndexItem class, though it is abstract.
 * @param text The IndexItem's text to be stored and tokenized.
 * @param allowCopies Whether or not duplicate words should be kept when tokenizing.
 */
IndexItem::IndexItem(const string text, bool allowCopies)
:tokens(text, allowCopies)
{
	//cout << "Index Item Constructed" << endl;
	//cout << "Testing content: " << content() << endl;
}

IndexItem::~IndexItem(){
}

/**
 * @return The original text of the IndexItem
 */
const string& IndexItem::content() const{
	return tokens.getText();
}

/**
 * @return A vector of the tokenized words of the text.
 */
const vector<string>& IndexItem::getTokens() const{
	return tokens.getTokens();
}

void IndexItem::setContent(string text, bool allowCopies){
	tokens = WordTokenizer(text, allowCopies);
}

/**
 * The overriden insertion<< operator. Mainly used for debugging.
 * It calls the pure virtual function toString(), which must be overriden in all subclasses.
 * @param os The output stream.
 * @param i the IndexItem to be output.
 * @return An updated output stream.
 */
ostream & operator<< (ostream & os, const IndexItem & i){
	//cout << "Call me with " << typeid(i).name() << endl;
	os << i.toString();
	return os;
}

