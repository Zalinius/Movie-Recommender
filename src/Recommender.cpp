

#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <cassert>
#include "Date.h"
#include "Movie.h"
#include "TaskPrinter.h"

using namespace std;

int main(){
	cout << "Task 4: Recommender!" << endl;
	shared_ptr<Stopword> stopwords = TaskPrinter::setUpStopwords();

	vector<Movie*> movies = TaskPrinter::setUpMovies();
	DocumentIndexer& movieDatabase = TaskPrinter::setUpMovieDatabase(movies, stopwords);
	cout << "Movie database is set up with " << movieDatabase.getIndex().size() << " movies."<< endl;
	TaskPrinter::printMovieQuery(movieDatabase);

}
