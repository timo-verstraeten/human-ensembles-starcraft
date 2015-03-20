#include "ErrorLogger.h"

ErrorLogger::~ErrorLogger()
{
	if (m_outputFile.is_open()) {
		m_outputFile.close();
	}
}

ErrorLogger &ErrorLogger::instance()
{
	static ErrorLogger instance;
	return instance;
}

void ErrorLogger::assert(bool condition, const char* message)
{
	if (!condition) {
		if (!m_outputFile.is_open()) {
			m_outputFile.open("error.log", std::ios::out | std::ios::app);
		}
		m_outputFile << message << std::endl;
	}
}