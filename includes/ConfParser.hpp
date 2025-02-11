#ifndef CONFPARSER_HPP
#define CONFPARSER_HPP

#include "usefull.hpp"
#include "BlockServer.hpp"
# include <iostream>
# include <fstream>
# include <vector>
# include <map>

class BlockServer;

class ConfParser {
	public:

		static int countLineFile;

		ConfParser(void);
		~ConfParser(void);

		void parsing(const std::string &filename);

		/* METHODS */
		void checkDoubleServerName();
		bool BlockServerBegin(std::vector<std::string> tokens);
		void ServersListens();

		/* CHERCKERS */
		static	std::vector<std::string>	supportedMethods;
		static	bool						CheckerMethod(std::string method);
		static	std::vector<std::string>	supportedHttpVersions;
		static	bool						CheckerHttpVersion(std::string version);

		/* GETTERS */
		std::map<std::string, std::vector<BlockServer> > getConfigs( void ) const { return _configs; }
		std::map<std::string, std::vector<BlockServer> > &getServers( void ) { return _configs; }

		void printServers();

	private:
		std::string _filename;

		std::vector<BlockServer> _servers;
		std::map<std::string, std::vector<BlockServer> > _configs;

		/* GETTERS */
		static std::vector<std::string>	_getSupportedMethods(void);
		static std::vector<std::string>	_getSupportedHttpVersions(void);

};

#endif
