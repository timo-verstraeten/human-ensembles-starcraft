#include "ErrorLogger.h"

ErrorLogger* ErrorLogger::m_instance = 0; 

ErrorLogger::~ErrorLogger()
{
	if (m_outputFile.is_open()) {
		m_outputFile.close();
	}
	m_instance = 0;
}

ErrorLogger* ErrorLogger::instance()
{
	if (!m_instance) {
		m_instance = new ErrorLogger();
	}
	return m_instance;
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