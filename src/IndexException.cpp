/*
 * IndexException.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: Nicole Parmentier
 */

#include "IndexException.h"


/**
 * Default exception constructor, gives a generic error message: "Index Exception"
 */
IndexException::IndexException() : errorMessage("Index Exception") {}

/**
 * Parameterized exception constructor, gives a custom error message with the parameter
 * @param name prints in the error message: name + " not found."
 */
IndexException::IndexException(string name) : errorMessage(name + " not found.") {}

/**
 * Default destructor
 */
IndexException::~IndexException() {}

/**
 * Gives a description of the exception based on how it was constructed
 * @return the error message for the exception
 */
const char* IndexException::what() const throw(){
	return errorMessage.c_str();
}
