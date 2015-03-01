#pragma once
#ifndef INC_ERROR_LOGGER_H
#define INC_ERROR_LOGGER_H

#include <fstream>

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