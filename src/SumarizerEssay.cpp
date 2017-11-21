//============================================================================
// Name        : Sumarizer.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "TaskPrinter.h"
using namespace std;

int main() {

	cout << "Task 3" << endl;

	vector<string>& files = TaskPrinter::setUpFiles();
	SentenceIndexer& library = TaskPrinter::setUpSentences(files);
	cout << endl;
	string s = TaskPrinter::readQuestion();

	int length;
	cout << endl << "What is the maximum essay length in words? ";
	cin >> length;

	vector<QueryResult> essay = library.query(s,length);

	for (vector<QueryResult>::iterator it = essay.begin(); it != essay.end(); ++it){
		cout << (it->getI())->content();
	}



	return 0;
}
