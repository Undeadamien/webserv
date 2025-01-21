#ifndef BLOCKLOCATION_HPP
#define BLOCKLOCATION_HPP

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

# include "ConfigParser.hpp"
# include "usefull.hpp"

enum e_boolMod
{
    FALSE,
    TRUE
};

class BlocLocation
{
	private:
		std::string	_path;
		std::string _root;
		std::pair<int, std::string> _rewrite;
		std::string _alias;
		std::vector<std::string> _indexes;
		std::vector<e_Methods> _allowedMethods;
		e_boolMod _autoindex;
		std::map<std::string, std::string> _cgiExtension;
		std::string _uploadPath;

		// divers
		std::map<std::string, int> _counterView;
		std::string _filename;

		bool isValidLocation(std::vector<std::string>& tokens, std::string& key);

		// Methods
		void incrementCounter(const std::string& key) { _counterView[key]++; }
		void DoubleLineChecker();
		void setDefaultValues();
		e_boolMod strToBool(std::string &str);

		// Adders
		void addAllowedMethods(std::vector<std::string> &tokens);
		void addIndexes(std::vector<std::string>& token);
		void addCgiExtension(std::vector<std::string>& token);

	public:
		BlocLocation(std::string filename);
		BlocLocation(const BlocLocation &copy);
		BlocLocation &operator=(const BlocLocation &copy);
		~BlocLocation();

		//parser
		BlocLocation getLocationConfig(std::ifstream &configFile, std::string &path);

		// Getters
		const std::string &getPath() const { return _path; }
		const std::string &getRoot() const { return _root; }
		const std::pair<int, std::string> &getRewrite() const { return _rewrite; }
		const std::string &getAlias() const { return _alias; }
		const std::vector<std::string> &getIndexes() const { return _indexes; }
		const std::map<std::string, std::string> &getCGI() const { return _cgiExtension; }
		const std::vector<std::string> &getFiles() const { return _indexes; }
		const std::vector<e_Methods> &getAllowedMethods() const { return _allowedMethods; }
		std::string	getCgiPath(const std::string &path) const { return _cgiExtension.at(path); }
		e_boolMod getAutoIndex() const { return _autoindex; }


};



#endif
