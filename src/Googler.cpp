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
	cout << "Task 2" << endl;

	shared_ptr<Stopword> stopwords = TaskPrinter::setUpStopwords();
	vector<string>& files = TaskPrinter::setUpFiles();

	DocumentIndexer& library = TaskPrinter::setUpLibrary(files, stopwords);
	TaskPrinter::printQuery(library);

	return 0;
}
