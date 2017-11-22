//
//#include <iostream>
//#include "TaskPrinter.h"
//#include "Stopword.h"
//using namespace std;
//
//int main() {
//
//	cout << "Task 3" << endl;
//
//	shared_ptr<Stopword> stopwords = TaskPrinter::setUpStopwords();
//
//	vector<string>& files = TaskPrinter::setUpFiles();
//
//	SentenceIndexer& library = TaskPrinter::setUpSentences(files, stopwords);
//	cout << endl;
//	string s = TaskPrinter::readQuestion();
//
//	int length;
//	cout << endl << "What is the maximum essay length in words? ";
//	cin >> length;
//
//	vector<QueryResult> essay = library.query(s,length);
//
//	for (vector<QueryResult>::iterator it = essay.begin(); it != essay.end(); ++it){
//		cout << (it->getI())->content() << endl;
//	}
//
//
//
//	return 0;
//}
