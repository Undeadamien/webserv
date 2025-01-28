#include "BlockLocation.hpp"

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
		_counterBase = copy._counterBase;
		_filename = copy._filename;
	}
	return (*this);
}

/*_____      _            _
 |  __ \    (_)          | |
 | |__) | __ ___   ____ _| |_ ___
 |  ___/ '__| \ \ / / _` | __/ _ \
 | |   | |  | |\ V / (_| | ||  __/
 |_|   |_|  |_| \_/ \__,_|\__\___|

                                   */


void BlockLocation::addValidMethod(std::vector<std::string> &tokens)
{
	e_Methods met;

	incrementCounter("allowedMethods");
	for (size_t i = 1; i < tokens.size(); i++)
	{
		std::string token = tokens[i];
		if (ConfParser::CheckerMethod(token) == false)
		{
			Log::log(Log::FATAL, "Invalid method: \"%s\" in file: %s:%d", token.c_str(), _filename.c_str(), ConfParser::countLineFile);
			exit(Log::FATAL);
		}
		else if (token == "POST")
			met = POST;
		else if (token == "DELETE")
			met = DELETE;
		else
			met = GET;
		if (std::find(_allowedMethods.begin(), _allowedMethods.end(), met) != _allowedMethods.end())
		{
			Log::log(Log::FATAL, "Duplicate method: \"%s\" in file: %s:%d", token.c_str(), _filename.c_str(), ConfParser::countLineFile);
			exit(Log::FATAL);
		}
		_allowedMethods.push_back(met);
	}
}

void BlockLocation::addIndexes(std::vector<std::string> &token)
{
	incrementCounter("indexes");
	for (size_t i = 1; i < token.size(); i++)
	{
		if (token[i][0] != '/')
		{
			Log::log(Log::FATAL, "Invalid index: \"%s\" in file: %s:%d", token[i].c_str(), _filename.c_str(), ConfParser::countLineFile);
			exit(Log::FATAL);
		}
		_indexes.push_back(token[i]);
	}
}

void BlockLocation::addCgiExtension(std::vector<std::string> &token)
{
	if (token.size() != 3)
	{
		Log::log(Log::FATAL, "Invalid cgi extension in file: %s:%d. Expected 3 elements, got %zu",
				 _filename.c_str(), ConfParser::countLineFile, token.size());
		exit(Log::FATAL);
	}

	if (token[1][0] != '.')
	{
		Log::log(Log::FATAL, "Invalid cgi extension: \"%s\" in file: %s:%d. Must start with '.'",
				 token[1].c_str(), _filename.c_str(), ConfParser::countLineFile);
		exit(Log::FATAL);
	}

	if (_cgiExtension.find(token[1]) != _cgiExtension.end())
	{
		Log::log(Log::FATAL, "Duplicate cgi extension: \"%s\" in file: %s:%d",
				 token[1].c_str(), _filename.c_str(), ConfParser::countLineFile);
		exit(Log::FATAL);
	}

	_cgiExtension[token[1]] = token[2];

	incrementCounter("cgiExtension");
}

/* Methods */

e_boolMod BlockLocation::strToBool(std::string &str)
{
	if (str == "on")
		return (TRUE);
	else if (str == "off")
		return (FALSE);
	else
	{
		Log::log(Log::FATAL, "Invalid autoindex value: \"%s\" in file: %s:%d", str.c_str(), _filename.c_str(), ConfParser::countLineFile);
		exit(Log::FATAL);
	}
}

void BlockLocation::setDefaultValues()
{
	_counterBase["root"] = 0;
	_counterBase["alias"] = 0;
	_counterBase["allowedMethods"] = 0;
	_counterBase["autoindex"] = 0;
	_counterBase["upload_path"] = 0;
}

void BlockLocation::DoubleLineChecker()
{
	for (auto &it : _counterBase)
	{
		if (it.second > 1)
		{
			Log::log(Log::FATAL, "Duplicate key: \"%s\" in file: %s:%d", it.first.c_str(), _filename.c_str(), ConfParser::countLineFile);
			exit(Log::FATAL);
		}
	}
}

void BlockLocation::DoubleLineChecker()
{
	std::map<std::string, int>::iterator it;

	for (it = _counterBase.begin(); it != _counterBase.end(); ++it)
		if (it->second > 1)
			Log::log(Log::FATAL, "Duplicate line in location context: %s", it->first.c_str());

	if (_counterBase["root"] > 0 && _counterBase["alias"] > 0)
		Log::log(Log::FATAL, "Alias and Root can't be set in same location bloc %s", _path.c_str());
}

bool BlockLocation::ValidLocationChecker(std::vector<std::string> &tokens, std::string &key)
{
	if (tokens.size() < 2)
		return false;
	if (key == "root" && tokens.size() == 2)
		setRoot(tokens[1]);
	else if (key == "autoindex")
		setAutoIndex(strToBool(tokens[1]));
	else if (key == "return" && tokens.size() == 3)
		setRewrite(tokens);
	else if (key == "alias" && tokens.size() == 2)
		setAlias(tokens[1]);
	else if (key == "allow_methods")
		addValidMethod(tokens);
	else if (key == "index")
		addIndexes(tokens);
	else if (key == "cgi_extension" && tokens.size() == 3)
		addCgiExtension(tokens);
	else if (key == "upload_path" && tokens.size() == 2)
		setUploadPath(tokens[1]);
	else
		return false;
	return true;
}

/*_____  _    _ ____  _      _____ _____
 |  __ \| |  | |  _ \| |    |_   _/ ____|
 | |__) | |  | | |_) | |      | || |
 |  ___/| |  | |  _ <| |      | || |
 | |    | |__| | |_) | |____ _| || |____
 |_|     \____/|____/|______|_____\_____|

                                          */

BlockLocation BlockLocation::getLocationConfig(std::ifstream &configFile, std::string &path)
{
	std::string line;
	std::vector<std::string> tokens;
	std::string key;
	bool isCloseLocation = false;

	setPath(path);
	while (std::getline(configFile, line))
	{
		ConfParser::countLineFile++;
		line = trimLine(line);
		if (line.empty() || line[0] == '#')
			continue;
		tokens = split(line, ' ');
		key = tokens[0];
		if (key[0] == '}' && key.size() == 1 && tokens.size() == 1)
		{
			isCloseLocation = true;
			break;
		}
		if (ValidLocationChecker(tokens, key))
			continue;
		else
			Log::log(Log::FATAL, "Invalid line: \"%s\" in file: %s:%d", line.c_str(), _filename.c_str(), ConfParser::countLineFile);
	}
	if (!isCloseLocation && !EmptyFileChecker())
		Log::log(Log::FATAL, "Missing } in file: %s:%d", _filename.c_str(), ConfParser::countLineFile);
	DoubleLineChecker();
	setDefaultValues();
	return (*this);
}
