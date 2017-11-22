/***
 * TaskPrinter.cpp
 *
 */


#include "TaskPrinter.h"
#include "Indexer.h"
#include "IndexItem.h"
#include "Stopword.h"
#include "Document.h"
#include "Sentence.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <math.h>
#include <algorithm>

using namespace std;

// ->Handles all the printing and formatting of the index, queries, etc.

/**
 * Read in stopwords, create the Stopword object which stores the list of stopwords
 * @return the newly created Stopword object
 */
shared_ptr<Stopword> TaskPrinter::setUpStopwords() {

	shared_ptr<Stopword> stopwords;
	bool done = false;
	while (!done){
		try{
			string stopWordsFile;
			cout << "Enter the stopwords file name: ";
			cin >> stopWordsFile;
			stopwords = make_shared<Stopword>(stopWordsFile.c_str());
			done = true;
		}
		catch (IndexException& e)
		{
			cout << e.what() << endl;
		}
	}
	return stopwords;
}


/**
 * Read in the configuration file (which contains the file names to be read) and load in File names
 * @return the vector<string> of file names
 */
vector<string>& TaskPrinter::setUpFiles(){

	ifstream fin = setUpFileStream("configuration");

	vector<string>* fileNames = new vector<string>();
	string name;
	unsigned int failCount = 0;
	while(getline(fin, name)){
		try{
			Document d("docs/" + name,true);
			fileNames->push_back("docs/" + name);
		}
		catch (IndexException& e){
			failCount++;
		}

	}
	fin.close();

	//User may abort the program if they think too few files have been read in successfully
	if (failCount > 0){
		char proceed = 'n';
		cout << fileNames->size()<< "/" << fileNames->size() + failCount  << " files read in. Proceed? Y/N" << endl;
		cin >> proceed;
		if (tolower(proceed) == 'n')
			exit(0);
	}

	return *fileNames;
}


/**
 * Create a Document object for every fileName that was read in, put it into the Indexer library
 * Builds up the dictionary of Library with the Term(s) from each Document
 * @param fileNames the vector<string> of file names to be read from
 * @return a DOcumentIndexer which stores the documents found from fileNames
 */
DocumentIndexer& TaskPrinter::setUpLibrary(vector<string>& fileNames, shared_ptr<Stopword> stopwords, bool omitStopwords){

	DocumentIndexer* library = new DocumentIndexer(fileNames.size(), stopwords, omitStopwords);
	for (vector<string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it){
		try{
			Document* d = new Document(*it,true);
			d >> *library;
		}
		catch (IndexException& e){
			//do nothing, this shouldn't actually ever happen
		}
	}

	return *library;
}

/**
 * Interacts with the user to make and display queries for Task 2, in googler.cpp
 * @param library the DocumentIndexer the user will query
 */
void TaskPrinter::printQuery(DocumentIndexer library)
{
	shared_ptr<Stopword> stopwords = library.getStopwords();
	bool newQuery;
	do
	{
		string s;
		unsigned int n;
		cout << "Enter number of desired search results: ";
		cin >> n;
		cout << "Enter string to be queried: ";
		getline(cin, s);
		s = "";
		getline(cin, s);

		vector<QueryResult>& result = library.query(s, n);
		cout << "\nQuery successful" << endl;

		for (unsigned int i = 0; i != result.size(); ++i)
		{
			cout << (i+1) << "- File: " << dynamic_cast<Document*>(result[i].getI())->getFileName() << ", score: " << result[i].getScore() << endl;
		}

		char answer;
		cout << "\nNew query? (Y/N): ";
		cin >> answer;
		if (tolower(answer) == 'n')
			newQuery = false;
		else
			newQuery = true;

	} while (newQuery);
}

/**
 * Reads in the question file name for Task 3, in summarizer.cpp
 * @return The cleaned version (punctuation removed) of the question to be queried
 */
string TaskPrinter::readQuestion() {
	string questionFileName;
	Document* d;
	bool done = false;
	while (!done){
		try{
			cout << "Enter the question file name: ";
			cin >> questionFileName;
			d = new Document(questionFileName, true);
			done = true;
			cout << questionFileName << " loaded." << endl;
		}
		catch (IndexException& e)
		{
			cerr << e.what();
		}
	}

	string cleanQuestion = "";
	for(vector<string>::const_iterator it = d->getTokens().begin(); it != d->getTokens().end(); ++it){
		cleanQuestion += *it;
		cleanQuestion += " ";
	}

	delete d;

	return cleanQuestion;
}

/**
 * Similar to setUpLibrary() for Task 1, fills a SentenceIndexer's index and dictionary for Task 2
 * @param fileNames the vector<string> of file names to be read from
 * @return a SentenceIndexer which stores the documents found from fileNames
 */
SentenceIndexer& TaskPrinter::setUpSentences(vector<string>& fileNames, shared_ptr<Stopword> stopwords){
	vector<Document*> docs;
	int sentenceCount = 0;
	for (vector<string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it){
		try{
			Document* d = new Document(*it,true);
			docs.push_back(d);
			sentenceCount += d->getSentences().size();
		}
		catch (IndexException& e){
			//do nothing
		}
	}

	SentenceIndexer* library = new SentenceIndexer(sentenceCount, stopwords, true);

	unsigned short docNo = 0;
	for(vector<Document*>::iterator it = docs.begin(); it != docs.end(); ++it){
		for(vector<Sentence>::iterator it2 = (**it).getSentences().begin(); it2 != (*it)->getSentences().end(); ++it2){
			it2->setDoc(docNo);
			&(*it2) >> *library;
		}
		++docNo;
	}

	//library->normalize();

	return *library;
}


/**
 * Prints the tables required for Task 1
 * @param library
 * @param fileNames the vector<string> of fileNames whose word counts and weights are being displayed
 * @param withoutStops whether of not stopwords will be printed
 * @param stopwords the object storing the stopwords
 */
void TaskPrinter::printIndex(DocumentIndexer library, vector<string>& fileNames, bool withoutStops, shared_ptr<Stopword> stopwords){
	//Header
	size_t longestWord = longest(library.getDictionary());
	const string DICTIONARY = "Dictionary";
	cout << "\n\n";
	if (longestWord < DICTIONARY.length()) //To fit "Dictionary"
		longestWord = DICTIONARY.length();

	cout << "*Printing Dictionary";
	if(withoutStops)
		cout <<" ignoring Stopwords";
	cout << "*";
	cout << endl << left << setw(longestWord + 1) << DICTIONARY;


	int docCount = 1;
	for (vector<string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it, ++docCount)
		cout << setw(4) << right << "Doc" << docCount; //Width fits any doc number with a space between columns
	cout << setw(6) << right << "Df";
	cout << "\n";

	vector<unsigned short> noStopWordTotals(fileNames.size());
	set<Term> dict = library.getDictionary();
	vector<IndexItem*> docIndex = library.getIndex();
	//Printing the words and counts
	{
		int docCount = 1;
		for(set<Term>::const_iterator it = dict.begin(); it != dict.end(); ++it){	//iterate through each term

			string word = it->getWord();
			if(!((*stopwords)(word) && withoutStops)){
				cout << left << setw(longestWord + 1) << " " + word;
				for(vector<IndexItem*>::const_iterator it2 = docIndex.begin(); it2 != docIndex.end(); ++it2){	//iterate through each document
					unsigned short toPrint = (*it2)->termFrequency(word);
					cout << right << setw(findn(docCount) + 4) << toPrint;	//width is +4 for " Doc"
					noStopWordTotals.at(docCount-1) += toPrint;
					++docCount;
				}
				cout << setw(findn(docCount) + 4) << right << it->getDocumentFrequency();
				cout << "\n";
				docCount = 1;
			}
		}
	}

	//Totals

	//adjust for stopwords or not
	cout << left << setw(longestWord + 1) << "Total";
	docCount = 1;
	if(withoutStops){
		for(vector<unsigned short>::const_iterator it = noStopWordTotals.begin(); it != noStopWordTotals.end(); ++it){
			cout << right << setw(findn(docCount) + 4) << *it; //width is +4 for " Doc"
			++docCount;
		}
	}
	else{
		for(vector<IndexItem*>::const_iterator it = library.getIndex().begin(); it != library.getIndex().end(); ++it){
			cout << right << setw(findn(docCount) + 4) << (*it)->getTokens().size(); //width is +4 for " Doc"
			++docCount;
		}
	}

	//Weights

	docCount = 1;

	cout <<endl <<endl;
	cout << endl << left << setw(longestWord + 1) << "Weights   ";

	for (vector<string>::const_iterator it = fileNames.begin(); it != fileNames.end(); ++it, ++docCount)
		cout << setw(4) << right << "Doc" << docCount; //Width fits any doc number with a space between columns
	cout << endl;

	docCount = 1;

	float h = library.getIndex().size();
	for(set<Term>::const_iterator it = dict.begin(); it != dict.end(); ++it){
		string word = it->getWord();
		if(!(withoutStops && (*stopwords)(word))){
			cout << left << setw(longestWord + 1) << " " + word;
			cout.flush();
			for(vector<IndexItem*>::const_iterator it2 = docIndex.begin(); it2 != docIndex.end(); ++it2){
				float score = 0;
				if ((*it2)->termFrequency(word) != 0)
					score = (1+log((*it2)->termFrequency(word)))*log(h/(float)it->getDocumentFrequency());

				cout << right << setw(findn(docCount) + 4) << setprecision(2) << score;	//width is +4 for " Doc"
				cout.flush();
				noStopWordTotals.at(docCount-1) += (*it2)->termFrequency(word);
				++docCount;
			}
			cout << "\n";
			docCount = 1;
		}
	}
}


/**
 * Printing out the Legend for Task 1
 * @param fileNames the file names to be printed in the legend
 */
void TaskPrinter::printLegend(const vector<string> &fileNames)
{

	cout << "\n*Document Legend*" << endl;
	int maxSize = findn(fileNames.size());
	for (size_t i = 0; i != fileNames.size(); ++i)
	{
		cout << "Doc" << setw(maxSize + 1) << right << (i + 1) << ": " << fileNames.at(i) << endl;
	}

}


/**
 * A helper method for formatting when printing out document tables, gives the number of digits in an integer
 * @param num the integer whose length we wish to know
 * @return the number of digits in num
 */
size_t TaskPrinter::findn(int num)
{
	unsigned int n = 0;
	while (num) {
		num /= 10;
		n++;
	}
	return n;
}

/**
 * A helper method for formatting when printing out document tables, finds the length of the longest word in the dictionary
 * @param dictionay the dictionary whose entries are checked
 * @return the number of digits in the longest word in the dictionary
 */
size_t TaskPrinter::longest(set<Term> dictionary){
	size_t longest = 0;
	for(set<Term>::const_iterator it = dictionary.begin(); it != dictionary.end(); ++it){
		if(it->getWord().length() > longest)
			longest = it->getWord().length();
	}
	return longest;
}

/**
 * Sets up an ifstream, asking for a new file name if the given name cannot be used to establish an ifstream
 * @param desiredFile the file name used to set up the ifstream
 * @return the ifstream set up with the user-provided file name
 */
ifstream TaskPrinter::setUpFileStream(string desiredFile){
	ifstream ifs;
	bool done = false;
	while (!done){
		try{
			string fileName;
			cout << "Enter the " << desiredFile << " file name: ";
			cin >> fileName;
			ifs = ifstream(fileName.c_str());
			if(!ifs)
				throw IndexException(fileName);
			done = true;
			cout << fileName << " loaded." << endl;
		}
		catch (IndexException& e)
		{
			cout << e.what() << endl;
		}
	}
	return ifs;
}

/**
 * Sets up a vector of movies created from the movie metadata file and the plot summaries file
 * @return a vector of pointers to the created Movie objects
 */
vector<Movie*>& TaskPrinter::setUpMovies(){

	ifstream ifs = setUpFileStream("movie metadata");

	//Load in all movie metadata
	string temp;
	vector<Movie> movies;
	while(getline(ifs, temp)){
		movies.push_back(Movie(temp));
	}
	ifs.close();

	cout << "There are " << movies.size() << " metadata." << endl;

	//Sort all movie metadata for log(n) access
	sort(movies.begin(), movies.end());

	ifstream ifs2 = setUpFileStream("movie summaries");

	//Load in movie summaries
	string temp2;
	while(getline(ifs2, temp2)){
		unsigned int iD;
		char c;
		string description;
		stringstream ss(temp2);
		ss >> iD;
		ss >> noskipws >> c;
		getline(ss, description);

		pair<vector<Movie>::iterator, vector<Movie>::iterator> bounds = equal_range(movies.begin(), movies.end(), Movie("", "", iD, Date(2010)));
		if(bounds.second - bounds.first == 1){
			bounds.first->setContent(description, true);
		}
		else{		//Ignore movie summaries that have no metadata.
		}
	}
	ifs2.close();
	cout << "Movie summaries loaded!\n";

	vector<Movie*>* m2 = new vector<Movie*>;
	//Remove metadata with no summary
	for(vector<Movie>::iterator it = movies.begin(); it != movies.end();++it){
		if(it->content() != ""){
			m2->push_back(new Movie(*it));
		}
	}
	movies.clear();

	cout << "There are now " << m2->size() << " complete and sorted movies!" << endl;
	return *m2;

}

/**
 * Similar to setUpLibrary() for Task 1, and setUpSentences for Task 2, fills the DocumentIndexer with Movie objects
 * @param movies the vector<Movie*> of Movie pointers to be databased
 * @param stopwords a shared_ptr<Stopword> which determines which words (the stopwords) are to be excluded from the plot query
 * @return a DocumentIndexer which stores the movies from the vector argument
 */
DocumentIndexer& TaskPrinter::setUpMovieDatabase(vector<Movie*>& movies, shared_ptr<Stopword> stopwords){
	DocumentIndexer* movieDatabase = new DocumentIndexer(movies.size(), stopwords, true);
	cout << "Initializing database..." << endl;
	for (vector<Movie*>::const_iterator it = movies.begin(); it != movies.end(); ++it){
		*it >> *movieDatabase;
	}
	return *movieDatabase;
}


/**
 * Takes the user's movie query and gives back the top n recommended movies based on the query movie's plot
 * @param movieDatabase the database of movies to be queried
 */
void TaskPrinter::printMovieQuery(DocumentIndexer& movieDatabase){
	shared_ptr<Stopword> stopwords = movieDatabase.getStopwords();
	bool newQuery;
	do
	{
		string s;
		vector<IndexItem*>::const_iterator found;
		bool done = false;
		getline(cin, s);
		while (!done){
			try{
				cout << "Enter the name of your movie: ";
				s = "";
				getline(cin, s);

				found = find_if(movieDatabase.getIndex().cbegin(), movieDatabase.getIndex().cend(),

						[&s] (IndexItem* item) {
					return (dynamic_cast<Movie*>(item))->getName() == s;
				}
				);
				if (found == movieDatabase.getIndex().cend())
					throw IndexException(s);
				else
					done = true;
			}
			catch (IndexException& e)
			{
				cout << e.what() << endl;
			}
		}

		unsigned int n;
		cout << "Enter number of desired search results: ";
		cin >> n;

		vector<QueryResult>& result = movieDatabase.movieQuery(*found, n);
		cout << "Recommendations:" << endl << endl;
		for (unsigned int i = 1; i != n+1; ++i)
		{
			cout << (i) << "- " << (*dynamic_cast<Movie*>(result[i].getI())) << endl;
		}

		char answer;
		cout << "\nFeeling brave enough for a new query? (Y/N): ";
		cin >> answer;
		if (tolower(answer) == 'n')
			newQuery = false;
		else
			newQuery = true;

	} while (newQuery);
}
