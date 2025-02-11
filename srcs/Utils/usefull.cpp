#include "usefull.hpp"

#include "ConfParser.hpp"
#include "Log.hpp"

std::string  ullToStr(unsigned long long ull)
{
	std::ostringstream oss;
	oss << ull;
	return (oss.str());
}

std::string itos(int value)
{
	std::stringstream ss;
	ss << value;
	return (ss.str());
}

bool EmptyFileChecker()
{
	return (ConfParser::countLineFile == 0);
}

std::string ltrim(std::string str, std::string set)
{
	if (set.empty())
		return (str);
	size_t start = str.find_first_not_of(set);
	if (start == std::string::npos)
		return (str);
	return (str.substr(start));
}
std::string rtrim(std::string str, std::string set)
{
	if (set.empty())
		return (str);
	size_t end = str.find_last_not_of(set);
	if (end == std::string::npos)
		return (str);
	return (str.substr(0, end + 1));
}
std::string trim(std::string str, std::string set)
{
	if (set.empty())
		return (str);
	size_t start = str.find_first_not_of(set);
	size_t end = str.find_last_not_of(set);
	if (start == std::string::npos || end == std::string::npos)
		return (str);
	return (str.substr(start, end - start + 1));
};

std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);	// Utilise un flux pour découper la chaîne
	std::string token;
	while (std::getline(ss, token, delimiter))
	{
		tokens.push_back(token);  // Ajoute chaque partie au vecteur
	}
	return tokens;
}

std::string intToString(int value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::string UIntToString(unsigned int value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}
