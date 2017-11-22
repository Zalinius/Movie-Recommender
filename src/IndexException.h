/*
 * Override the virtual function what() to provide an explanation of the exception. Change any existing
code in your classes to use the new exception class in case of errors. Make sure you process the
exceptions (try...catch) so that your main program does not terminate. In particular, for Task 4,
asking for recommendations based on a movie name that is not in the index will result in an exception.
 *
 */

#ifndef INDEXEXCEPTION_H_
#define INDEXEXCEPTION_H_

#include <iostream>
#include <exception>

using namespace std;

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
