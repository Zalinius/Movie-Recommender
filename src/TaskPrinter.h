/***
 * TaskPrinter.h
*/


#ifndef TASKPRINTER_H
#define TASKPRINTER_H

#include "Stopword.h"
#include "DocumentIndexer.h"
#include "SentenceIndexer.h"
#include "Movie.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// ->Handles all the printing and formatting of the index, queries, etc.

class TaskPrinter
{
	public:
		static shared_ptr<Stopword> setUpStopwords();
		static vector<string>& setUpFiles();
		static DocumentIndexer& setUpLibrary(vector<string>& fileNames, shared_ptr<Stopword> stopwords, bool omitStopwords);
		static SentenceIndexer& setUpSentences(vector<string>& fileNames, shared_ptr<Stopword> stopwords);
		static DocumentIndexer& setUpMovieDatabase(vector<Movie*>& movies, shared_ptr<Stopword> stopwords);
		static string readQuestion();	//belongs in TaskPrinter
		static vector<Movie*>& setUpMovies();

		static void printIndex(DocumentIndexer library, vector<string>& fileNames, bool withoutStops, shared_ptr<Stopword> stopwords);
		static void printWeightedIndex(DocumentIndexer library);
		static void printLegend(const vector<string>& fileNames);
		static void printQuery(DocumentIndexer library);
		static void printMovieQuery(DocumentIndexer& movieDatabase);
	private:
		static ifstream setUpFileStream(string desiredFile);
		static size_t findn(int num);
		static size_t longest(set<Term> dictionary);
};

#endif
