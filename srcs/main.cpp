#include "Arguments.hpp"
#include "ConfParser.hpp"
#include "test.hpp"

Server* glo_server;

int ConfParser::countLineFile = 0;

int main(int argc, char** argv, char** env)
{
	(void)env;
	//Server server;
	//glo_server = &server;

	if (!check_arguments(argc, argv)) return 1;

	ConfParser parser;
	parser.parsing(argv[1]);
	Log::log(Log::DEBUG, "Config File parsed.");

	MapServers _servers = parser.getConfigs();

	MapServers::iterator iter = _servers.begin();

	BlockServer server = iter->second[0];

	BlockLocation* location = server.getLocationByPath("/juiceee");
	if (location == NULL)
	{
		Log::log(Log::ERROR, "Location not found.");
		return 1;
	}
	location->printLocation();

	//Log::log(Log::DEBUG, "Server Config printed.");
	//try
	//{
    //    Log::log(Log::DEBUG, "Config File parsed.");
    //    if (Log::getLogDebugState())
	//	    parser.printServers();
	//	simple_server(parser, env);
	//}
	//catch (std::exception& e)
	//{
	//	return (EXIT_FAILURE);
	//}
	//Log::log(Log::DEBUG, "Server stopped.");
	return (EXIT_SUCCESS);
}
