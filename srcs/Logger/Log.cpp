#include "Log.hpp"
#include "usefull.hpp"

bool Log::_logState = DF_LOG_ON;
bool Log::_logFileState = DF_LOG_FILE_ON;
bool Log::_logDebugState = DF_DEBUG_MODE_ON;

std::string Log::_logFileName = Log::_generateLogFileName();

std::map<Log::LogStep, std::string> Log::_LogStepStr = Log::_selectLogStepStr();


std::map<Log::LogStep, std::string> Log::_LogStepColor = Log::_selecLogStepColor();

/*
#desc : Genrate the log file name

#return : std::string
*/
std::string Log::_generateLogFileName(void)
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << "webserv_" << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".log";
	return oss.str();
}

std::map<Logger::LogLevel, std::string> Logger::_generateLogLevelStr(void)
{
	std::map<Logger::LogLevel, std::string> logLevelStr;

	logLevelStr[Logger::FATAL] = "FATAL";
	logLevelStr[Logger::ERROR] = "ERROR";
	logLevelStr[Logger::INFO] = "INFO";
	logLevelStr[Logger::DEBUG] = "DEBUG";

	return (logLevelStr);
}
