#pragma once
#ifndef INC_ERROR_LOGGER_H
#define INC_ERROR_LOGGER_H

#include <fstream>

/**
 * Singleton to log errors to a file.
 * This should be used to report errors, as it is meant to replace asserts in the code (as they are unpossible to use in this project).
 */
class ErrorLogger
{
public:
	virtual ~ErrorLogger();

	static ErrorLogger &instance();
	void assert(bool condition, const char* message);
	
private:
	ErrorLogger() {};
	ErrorLogger(ErrorLogger const&) {};
	ErrorLogger& operator=(ErrorLogger const&) {};

	std::ofstream m_outputFile;
};

#endif // ERROR_LOGGER_H