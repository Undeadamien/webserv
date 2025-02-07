#ifndef BLOCKSERVER_HPP
# define BLOCKSERVER_HPP

# include "usefull.hpp"
# include "Log.hpp"
# include "ConfParser.hpp"
# include "BlockLocation.hpp"
# include "ListenIpConfParse.hpp"
#include <iostream>

class Log;
class ConfParser;
class BlockLocation;
class ListenIpConfParse;

# define DF_CLIENT_MAX_BODY 1048576 // 1MB
class BlockServer {
	private:
		std::map<std::string, ListenIpConfParse> _listens;
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
		bool DoubleLineChecker();
		void incrementCounter(const std::string &key) { _counterBase[key]++; }
		bool ValidServerChecker(std::vector<std::string>& tokens, std::string& key, std::ifstream &configFile);
		bool isStartBlockLocation(std::vector<std::string>& tokens);
		bool DoubleLocationChecker();
		void cleanPaths();

	public:
		BlockServer(std::string filename);
		BlockServer(void);
		BlockServer(const BlockServer &other);
		~BlockServer(void);

		BlockServer &operator=(const BlockServer &other);

		//// parsing
		//BlockServer getServerConfig(std::ifstream &file_config);

		// Getters
		const std::map<int, std::string> &getErrorPages() const { return _errorPages; }
		const std::vector<std::string> &getServerNames() const { return _serverNames; }
		std::vector<BlockLocation>* getLocations() { return &_locations; }
		const std::string &getRoot() const { return _root; }
		unsigned long long getClientMaxBodySize() const { return _clientMaxBodySize; }
		const std::map<std::string, ListenIpConfParse> &getListens() const { return _listens; }
		const std::vector<std::string> &getIndexes() const { return _indexes; }

		//	// Util
		bool isServerNamePresent(std::vector<std::string>& otherNames);


		//// Setters
		void setClientMaxBodySize(std::string clientMaxBodySize);
		void setRoot(const std::string &root);
		void setDefaultValue();
		void setLocations(const std::vector<BlockLocation> &locations) { _locations = locations; }
		void setErrorPages(const std::map<int, std::string> &errorPage) { _errorPages = errorPage; }

		//// Adders
		void addErrorPages(int errorCode, std::string file);
		void addLocation(const BlockLocation &locations) { _locations.push_back(locations); }
		void addListen(std::string &token);
		void addServerName(std::vector<std::string>& token);
		void addIndexes(std::vector<std::string>& token);


		//// Finders
		//BlockLocation*	findLocation(const std::string &uri);

		//// Print
		//void printServer(void);
		//void printListens();
		//void printPair(const std::string& label, const std::string& value);
		//void printInt(const std::string& label, int value);
		//void printVector(const std::string& label, const std::vector<std::string>& vec);
		//void printMap(const std::string& label, const std::map<int, std::string>& map);
};

#endif
