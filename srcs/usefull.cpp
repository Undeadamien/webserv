#include "usefull.hpp"

#include "Log.hpp"

std::string itos(int value)
{
	std::stringstream ss;
	ss << value;
	return (ss.str());
}

bool check_arguments(int argc, char **argv)
{
	if (argc == 2)	// et si parsing du premier argument fait
		return ();
	else if (argc > 2 && argc < 5)
	{
		check_options(argc, argv);
	}
	else
	{
		Log::log(Log::FATAL, "Bads arguments\n");
		return (false);
	}
}

bool filename_parser(const std::string &filename)
{
	if (filename.size() < 6)
	{
		Log::log(Log::FATAL, "Less than 6 caracters \n");
		return false;
	}
	if (filename.substr(filename.size() - 5) != ".conf")
	{
		Log::log(Log::FATAL, "Invalid extension\n");
		return false;
	}
	return true;
}

bool check_options(int argc, char **argv)
{
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
		else
		{
			Log::log(Log::FATAL, "Option : %s invalid\n", argv[i]);
			return false;
		}
	}
	return (true);
}
