#include "IndexException.h"



IndexException::IndexException() : errorMessage("Index Exception") {}

IndexException::IndexException(string name) : errorMessage(name + " not found.") {}

IndexException::~IndexException() {}

const char* IndexException::what() const throw(){
	return errorMessage.c_str();
}
