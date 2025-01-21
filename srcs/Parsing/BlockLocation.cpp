#include "BlockLocation.hpp"

void BlockLocation::setDefaultValues()
{
	_counterView["root"] = 0;
	_counterView["alias"] = 0;
	_counterView["allowedMethods"] = 0;
	_counterView["autoindex"] = 0;
	_counterView["upload_path"] = 0;
}

BlockLocation::BlockLocation(std::string filename) : _autoindex(FALSE), _filename(filename)
{
	setDefaultValues();
}

BlockLocation::BlockLocation(const BlockLocation &copy)
{
	*this = copy;
}

BlockLocation::~BlockLocation() {}

BlockLocation &BlockLocation::operator=(const BlockLocation &copy)
{
	if (this != &copy)
	{
		_path = copy._path;
		_root = copy._root;
		_rewrite = copy._rewrite;
		_alias = copy._alias;
		_indexes = copy._indexes;
		_allowedMethods = copy._allowedMethods;
		_autoindex = copy._autoindex;
		_cgiExtension = copy._cgiExtension;
		_uploadPath = copy._uploadPath;
		_counterView = copy._counterView;
		_filename = copy._filename;
	}
	return (*this);
}

void BlockLocation::addAllowedMethods(std::vector<std::string> &tokens)
{
	e_Methods met;

	incrementCounter("allowedMethods");
	for (size_t i = 1; i < tokens.size(); i++){
		std::string token = tokens[i];
/*		if (ConfigParser::isMethodSupported(token) == false)
			Logger::log(Logger::FATAL, "Invalid method: \"%s\" in file: %s:%d", token.c_str(), _filename.c_str(), ConfigParser::countLineFile);*/
		if (token == "GET")
			met = GET;
		else if (token == "POST")
			met = POST;
		else if (token == "DELETE")
			met = DELETE;
		else if (token == "PUT")
			met = PUT;
/*		else
			Logger::log(Logger::FATAL, "Invalid method: \"%s\" in file: %s:%d", token.c_str(), _filename.c_str(), ConfigParser::countLineFile);
		if (std::find(_allowedMethods.begin(), _allowedMethods.end(), met) != _allowedMethods.end())
			Logger::log(Logger::FATAL, "Duplicate method: \"%s\" in file: %s:%d", token.c_str(), _filename.c_str(), ConfigParser::countLineFile);*/
		_allowedMethods.push_back(met);
	}
}
