#include "usefull.hpp"

#include "ConfParser.hpp"
#include "Log.hpp"

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

std::string trimline(const std::string &str)
{
	// Trouve le début non-espace
	auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch)
								  { return std::isspace(ch); });
	// Trouve la fin non-espace
	auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch)
								{ return std::isspace(ch); })
				   .base();
	// Retourne la sous-chaîne "trimmée"
	return (start < end) ? std::string(start, end) : std::string();
}

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
