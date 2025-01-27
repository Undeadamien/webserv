#include "usefull.hpp"
#include "Log.hpp"

std::string itos(int value)
{
    std::stringstream ss;
    ss << value;
    return (ss.str());
}

bool check_logger(int argc, char **argv)
{
	if (argc == 2)
		return true;
	if (argc > 5)
	{
		Log::log(Log::FATAL, "Too many arguments");
		return (false);
	}
	Log::setLogState(true);
	for (int i = 2; i < argc; i++)
		{
			if (std::string(argv[i]) == "--log")
			{
				continue;
			}
			else if (std::string(argv[i]) == "--debug")
				Log::setLogDebugState(true);
			else if (std::string(argv[i]) == "--save")
				Log::setLogFileState(true);
			else {
					Log::log(Log::FATAL, "Option : %s invalid\n", argv[i]);
					return false;
			}
	}
	return (true);
}
