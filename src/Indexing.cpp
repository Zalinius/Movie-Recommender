//============================================================================
// Name        : Indexing.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "TaskPrinter.h"
using namespace std;

int main() {
	cout << "Task 1!" << endl;

	vector<string> fileNames = TaskPrinter::setUpFiles();

	shared_ptr<Stopword> stopwords = TaskPrinter::setUpStopwords();

	DocumentIndexer library = TaskPrinter::setUpLibrary(fileNames);


	TaskPrinter::printIndex(library, fileNames, false, stopwords);
	TaskPrinter::printIndex(library, fileNames, true, stopwords);
	TaskPrinter::printLegend(fileNames);


	return 0;
}
