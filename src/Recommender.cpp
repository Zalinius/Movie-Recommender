/*
 * Recommender.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: lapoi
 */

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

	vector<Movie> movies;

	cout << "Enter the movie titles file name: ";
	string movieFileName;
	cin >> movieFileName;

	//Load in all movie metadata
	ifstream ifs(movieFileName);
	string temp;
	while(getline(ifs, temp)){
		movies.push_back(Movie(temp));
	}
	ifs.close();

	cout << "All movie names loaded!" <<endl;
	cout << "There are " << movies.size() << " movies!" << endl;

	//Sort all movie metadata for log(n) access
	sort(movies.begin(), movies.end());
	cout << "Movies sorted" << endl;

	cout << "Search for film 5463!" << endl;
	pair<vector<Movie>::iterator, vector<Movie>::iterator> bounds = equal_range(movies.begin(), movies.end(), movies[5463]);
	cout << movies[5463] << endl;
	cout << *bounds.first << endl;

	cout << "Enter the movie summaries file name: ";
	string summariesFileName;
	cin >> summariesFileName;

	//Load in movie summaries
	ifstream ifs2(summariesFileName);
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

	vector<Movie> m2;
	//Remove metadata with no summary
	for(vector<Movie>::iterator it = movies.begin(); it != movies.end();++it){
		if(it->content() != ""){
			m2.push_back(*it);
		}
	}
	movies.clear();

	cout << "There are now " << m2.size() << " complete and sorted movies!" << endl;

}
