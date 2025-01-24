#include "Log.hpp"
#include "usefull.hpp"

bool Log::_logState = DF_LOG_ON;
bool Log::_logFileState = DF_LOG_FILE_ON;
bool Log::_logDebugState = DF_DEBUG_MODE_ON;

std::string Log::_logFileName = Log::_fileNameGenerator();

std::map<Log::LogStep, std::string> Log::_logStepMap = Log::_selectLogStepStr();


std::map<Log::LogStep, std::string> Log::_LogStepColor = Log::_selectLogStepColor();

/*
#desc : Genrate the log file name

#return : std::string
*/
std::string Log::_fileNameGenerator(void)
{
	std::time_t t = std::time(NULL);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "webserv_%Y-%m-%d_%H-%M-%S.log", std::localtime(&t));
	return (std::string(buffer));
}

std::map<Log::LogStep, std::string> Log::_selectLogStepColor(void)
{
	std::map<Log::LogStep, std::string> Colorization;

	Colorization[Log::FATAL] = RED;
	Colorization[Log::ERROR] = ORANGE;
	Colorization[Log::INFO] = GREEN;
	Colorization[Log::DEBUG] = CYAN;

	return (Colorization);
}

std::map<Log::LogStep, std::string> Log::_selectLogStepStr(void)
{
	std::map<Log::LogStep, std::string> logStepCouple;

	logStepCouple[Log::FATAL] = "FATAL";
	logStepCouple[Log::ERROR] = "ERROR";
	logStepCouple[Log::INFO] = "INFO";
	logStepCouple[Log::DEBUG] = "DEBUG";

	return (logStepCouple);
}

std::string Log::_logFormater(Log::LogStep level, const std::string msg, std::string time, bool color_on)
{
	std::string formatedMsg;

	if (color_on == true)
		formatedMsg += Log::getLogStepColor(level);
	formatedMsg += "[" + Log::getLogStepMap(level) + "]\t" + time + " : " + msg;
	if ((level == Log::FATAL) && errno != 0)
		formatedMsg += ": " + static_cast<std::string>(std::strerror(errno));
	if (color_on == true)
		formatedMsg += RESET;

	return (formatedMsg);
}

