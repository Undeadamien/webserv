#ifndef BLOCKSERVER_HPP
# define BLOCKSERVER_HPP

# include "usefull.hpp"
# include "Log.hpp"
# include "ConfParser.hpp"
# include "BlockLocation.hpp"
# include "ListenIpConfig.hpp"

class Log;
class ConfParser;
class BlockLocation;
class ListenIpConfig;

class BlockServer {
	private:
		std::map<std::string, ListenIpConfig> _listens;
		std::vector<std::string> _serverNames;
		std::vector<std::string> _indexes;
		std::string _root;
		unsigned long long _clientMaxBodySize;
		std::vector<BlockLocation> _locations;
		std::map<int, std::string> _errorPages;

		// divers
		std::string _filename;
		std::map<std::string, int> _counterBase;

			// Methods
		void DoubleLineChecker();
		void incrementCounter(const std::string &key) { _counterBase[key]++; }
		bool ValidServerChecker(std::vector<std::string>& tokens, std::string& key, std::ifstream &configFile);
		bool isStartBlocLocation(std::vector<std::string>& tokens);
		void DoubleLocationChecker();
		void cleanPaths();

	public:
};

#endif
