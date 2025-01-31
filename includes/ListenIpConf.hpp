#ifndef LISTENIPCONF_HPP
#define LISTENIPCONF_HPP

#include "Log.hpp"
#include "usefull.hpp"

class ListenIpConf
{
	private:
		std::string _ip;
		unsigned int _port;
		std::string _ipAndPort;

		// Checker
		bool checkIpPort();
		bool isValidIp(const std::string& ipStr);

	public:
		ListenIpConf();
		ListenIpConf(std::string token);
		~ListenIpConf();

		// Getters
		const std::string &getIp() const { return _ip; }
		unsigned int getPort() const { return _port; }
		const std::string getIpPortJoin() const { return _ipAndPort; }
};

#endif
