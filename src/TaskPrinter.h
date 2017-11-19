/***
 * TaskPrinter.h
*/


#ifndef TASKPRINTER_H
#define TASKPRINTER_H

#include "Stopword.h"
#include "DocumentIndexer.h"
#include "SentenceIndexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// ->Handles all the printing and formatting of the index, queries, etc.

class TaskPrinter
{
	public:
		static unique_ptr<Stopword> setUpStopwords();
		static vector<string>& setUpFiles();
		static DocumentIndexer& setUpLibrary(vector<string>& fileNames);
		static SentenceIndexer& setUpSentences(vector<string>& fileNames);
		static string readQuestion();	//belongs in TaskPrinter

		static void printIndex(DocumentIndexer library, vector<string>& fileNames, bool withoutStops, unique_ptr<Stopword> stopwords);
		static void printWeightedIndex(DocumentIndexer library);
		static void printLegend(const vector<string>& fileNames);
		static void printQuery(DocumentIndexer library);
	private:
		static size_t findn(int num);
		static size_t longest(vector<Term> dictionary);
};

#endif
