
#ifndef INDEXEXCEPTION_H_
#define INDEXEXCEPTION_H_

#include <iostream>
#include <exception>

using namespace std;

/**
 * This class is used to catch mistakes in various indexing functions, namely invalid file names (all tasks) and movie names (task 4)
 */
class IndexException : public exception {
public:
	IndexException();
	IndexException(string name);
	virtual ~IndexException();
	virtual const char * what () const throw();	//catch (const IndexException& myException)

private:
	string errorMessage;
};

#endif /* INDEXEXCEPTION_H_ */
