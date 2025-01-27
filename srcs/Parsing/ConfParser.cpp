#include "ConfParser.hpp"

std::vector<std::string> ConfParser::supportedMethods = ConfParser::_getSupportedMethods();

std::vector<std::string> ConfParser::supportedHttpVersions = ConfParser::_getSupportedHttpVersions();

ConfParser::ConfParser(void) {}

ConfParser::~ConfParser(void) {}

/*__  __      _   _               _
 |  \/  |    | | | |             | |
 | \  / | ___| |_| |__   ___   __| |___
 | |\/| |/ _ \ __| '_ \ / _ \ / _` / __|
 | |  | |  __/ |_| | | | (_) | (_| \__ \
 |_|  |_|\___|\__|_| |_|\___/ \__,_|___/

                                        */

std::vector<std::string> ConfParser::_getSupportedMethods(void)
{
	std::vector<std::string> methods;

	methods.push_back("GET");
	methods.push_back("POST");
	methods.push_back("DELETE");

	return (methods);
}

bool ConfParser::CheckerMethod(std::string method)
{
	return (std::find(ConfParser::supportedMethods.begin(), ConfParser::supportedMethods.end(), method) != ConfParser::supportedMethods.end());
}

/*_    _ _______ _______ _____
 | |  | |__   __|__   __|  __ \
 | |__| |  | |     | |  | |__) |
 |  __  |  | |     | |  |  ___/
 | |  | |  | |     | |  | |
 |_|  |_|  |_|     |_|  |_|

                                */

std::vector<std::string> ConfParser::_getSupportedHttpVersions(void)
{
	std::vector<std::string> versions;

	versions.push_back("HTTP/1.0");
	versions.push_back("HTTP/1.1");

	return (versions);
}

bool ConfParser::CheckerHttpVersion(std::string method)
{
	return (std::find(ConfParser::supportedHttpVersions.begin(), ConfParser::supportedHttpVersions.end(), method) != ConfParser::supportedHttpVersions.end());
}

//void ConfParser::printServers(void){
//	for (size_t i = 0; i < _servers.size(); i++)
//	{
//		std::cout << "============ SERVER " << i + 1 << " ===========\n"
//				  << std::endl;
//		_servers[i].printServer();
//		std::cout << std::endl;
//	}
//}

void ConfParser::checkDoubleServerName()
{

}

bool ConfParser::BlockServerBegin(std::vector<std::string> tokens){
	return ((tokens.size() == 2 && tokens[0] == "server" && tokens[1] == "{"))
				|| (tokens.size() == 1 && tokens[0] == "server{");
}

