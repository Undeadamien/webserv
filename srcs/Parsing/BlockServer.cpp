#include "BlockServer.hpp"

BlockServer::BlockServer(void) : _clientMaxBodySize(DF_CLIENT_MAX_BODY), _filename("")
{
	_counterBase["root"] = 0;
	_counterBase["clientMaxBodySize"] = 0;
}

BlockServer::BlockServer(std::string filename) : _clientMaxBodySize(DF_CLIENT_MAX_BODY), _filename(filename)
{
	_counterBase["root"] = 0;
	_counterBase["clientMaxBodySize"] = 0;
}

BlockServer::BlockServer(const BlockServer &other)
{
	*this = other;
}

BlockServer::~BlockServer(void) {}

BlockServer &BlockServer::operator=(const BlockServer &other)
{
	if (this != &other)
	{
		_listens = other._listens;
		_serverNames = other._serverNames;
		_indexes = other._indexes;
		_root = other._root;
		_clientMaxBodySize = other._clientMaxBodySize;
		_locations = other._locations;
		_errorPages = other._errorPages;
		_filename = other._filename;
		_counterBase = other._counterBase;
	}
	return *this;
}

/*_____      _            _
 |  __ \    (_)          | |
 | |__) | __ ___   ____ _| |_ ___
 |  ___/ '__| \ \ / / _` | __/ _ \
 | |   | |  | |\ V / (_| | ||  __/
 |_|   |_|  |_| \_/ \__,_|\__\___|

								   */

void BlockServer::DoubleLineChecker()
{
	std::map<std::string, int>::iterator it;

	for (it = _counterBase.begin(); it != _counterBase.end(); ++it)
		if (it->second > 1)
			Log::log(Log::FATAL, "Duplicate line in server context: %s", it->first.c_str());
}

bool BlockServer::isStartBlockLocation(std::vector<std::string> &tokens)
{
	return (tokens.size() == 3 && tokens[0] == "location" && tokens[2] == "{");
}

void BlockServer::DoubleLocationChecker()
{
	std::vector<BlockLocation>::iterator it;
	std::vector<BlockLocation>::iterator it2;

	for (it = _locations.begin(); it != _locations.end(); ++it)
	{
		for (it2 = it + 1; it2 != _locations.end(); ++it2)
		{
			if (it->getPath() == it2->getPath())
				Log::log(Log::FATAL, "Duplicate location: %s", it->getPath().c_str());
		}
	}
}

void BlockServer::cleanPaths()
{
	if (!_root.empty() && _root != "/" && _root[_root.size() - 1] == '/')
		_root.erase(_root.size() - 1);

	for (std::map<int, std::string>::iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
	{
		if (it->second != "/" && it->second[it->second.size() - 1] == '/')
			it->second.erase(it->second.size() - 1);
	}

	std::vector<BlockLocation>::iterator it;
	for (it = _locations.begin(); it != _locations.end(); ++it)
	{
		it->cleanPaths();
	}
}

/* _____ ______ _______ _______ ______ _____   _____
  / ____|  ____|__   __|__   __|  ____|  __ \ / ____|
 | (___ | |__     | |     | |  | |__  | |__) | (___
  \___ \|  __|    | |     | |  |  __| |  _  / \___ \
  ____) | |____   | |     | |  | |____| | \ \ ____) |
 |_____/|______|  |_|     |_|  |______|_|  \_\_____/

													  */

void BlockServer::setClientMaxBodySize(std::string clientMaxBodySize)
{
	if (clientMaxBodySize == "none")
		_clientMaxBodySize = 0;
	else
	{
		char *end;
		unsigned long long value = std::strtoull(clientMaxBodySize.c_str(), &end, 10);

		if (*end != '\0')
			throw std::invalid_argument("Invalid number format: " + clientMaxBodySize);

		_clientMaxBodySize = value;
	}
	_counterBase["clientMaxBodySize"]++;
}

void BlockServer::setRoot(const std::string &root)
{
	_root = root;
	_counterBase["root"]++;
}

void BlockServer::setDefaultValue()
{
	if (_listens.empty())
	{
		ListenIpConf listen("0.0.0.0:1234");
		_listens["0.0.0.0:1234"] = listen;
	}
	if (_root.empty())
		_root = "./www/main";
	if (_indexes.empty())
		_indexes.push_back("index.html");
}
