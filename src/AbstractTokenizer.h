#ifndef ABSTRACTTOKENIZER_H_
#define ABSTRACTTOKENIZER_H_

using namespace std;
#include <string>
#include <fstream>
#include <vector>

class Stopword;

/**
 * A abstract class for all tokenizer derived classes.
 */
class AbstractTokenizer{

public:
	AbstractTokenizer(const string text, const vector<string> words, bool allowCopies = true);
	virtual ~AbstractTokenizer();

	friend ostream & operator<< (ostream & os, AbstractTokenizer & t); //Mainly for debugging

	const string& getText() const;
	const vector<string>& getTokens() const;

	static bool isAbbreviation(string term);

protected:
private:
	string text;
	vector<string> words;
	bool allowCopies;

	static Stopword abbreviations;
	static string lastWord(const string &sentence);


	virtual string toString() const = 0;
	virtual const vector<string> tokenize(const string &text) = 0;
};





#endif /* ABSTRACTTOKENIZER_H_ */
