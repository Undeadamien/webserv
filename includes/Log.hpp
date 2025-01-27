#ifndef LOG_HPP
# define LOG_HPP

#include "usefull.hpp"
#include "colors.hpp"
#include <map>
#include <stdarg.h>
#include <ctime>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>

// Default parameters
/*#ifndef DF_LOG_ON
# define DF_LOG_ON false
#endif

#ifndef DF_LOG_FILE_ON
# define DF_LOG_FILE_ON false
#endif

#ifndef DF_DEBUG_MODE_ON
# define DF_DEBUG_MODE_ON false
#endif*/

class Log
{

	public:
		// Levels of log
		enum LogStep
		{
			DEBUG = 0,
			INFO,
			ERROR,
			FATAL,
		};

		static void log(LogStep level, const char *msg, ...);

		//SETTERS
		static void setLogState(bool state);
		static void setLogFileState(bool state);
		static void setLogDebugState(bool state);

		//GETTERS
		static bool getLogState(void);
		static bool getLogFileState(void);
		static bool getLogDebugState(void);
		static std::string getLogFileName(void);
		static std::string getLogStepMap(LogStep level);
		static std::string getLogStepColor(LogStep level);

	private:
		static bool _logState;
		static bool _logFileState;
		static bool _logDebugState;
		static std::string _logFileName;
		static std::map<Log::LogStep, std::string> _logStepMap;
		static std::map<Log::LogStep, std::string> _LogStepColor;

		//UTILS
		static std::string _fileNameGenerator(void);
		static std::map<Log::LogStep, std::string> _selectLogStepStr(void);
		static std::map<Log::LogStep, std::string> _selectLogStepColor(void);
		static std::string _logFormater(Log::LogStep level, const std::string msg, std::string time, bool color_on = true);
		static void _printMsgFormater(Log::LogStep level, const std::string msg, std::string time);
		static void _InLogFile(LogStep level, const char *msg, std::string time);

};

#endif
