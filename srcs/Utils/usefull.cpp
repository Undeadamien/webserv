#include "usefull.hpp"
#include "ConfParser.hpp"
#include "Log.hpp"

int fdSocket = -1;


std::string ullToStr(unsigned long long ull)
{
	std::ostringstream oss;
	oss << ull;
	return (oss.str());
}

std::string ft_itos(int value)
{
	std::stringstream ss;
	ss << value;
	return (ss.str());
}

bool EmptyFileChecker() { return (ConfParser::countLineFile == 0); }

std::string ltrim(std::string str, std::string set)
{
	if (set.empty()) return (str);
	size_t start = str.find_first_not_of(set);
	if (start == std::string::npos) return (str);
	return (str.substr(start));
}
std::string rtrim(std::string str, std::string set)
{
	if (set.empty()) return (str);
	size_t end = str.find_last_not_of(set);
	if (end == std::string::npos) return (str);
	return (str.substr(0, end + 1));
}
std::string trim(std::string str, std::string set)
{
	if (set.empty()) return (str);
	size_t start = str.find_first_not_of(set);
	size_t end = str.find_last_not_of(set);
	if (start == std::string::npos || end == std::string::npos) return (str);
	return (str.substr(start, end - start + 1));
};

std::vector<std::string> split(const std::string& str, char delimiter)
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

int extractPort(const std::string& server)
{
	size_t colonPos = server.find(':');

	if (colonPos != std::string::npos)
	{
		try
		{
			return std::atoi((server.substr(colonPos + 1)).c_str());
		}
		catch (std::runtime_error& e)
		{
			Log::log(Log::ERROR, e.what());
			return -1;
		}
	}
	return -1;
}

std::string extractIp(const std::string& server)
{
    size_t colonPos = server.find(':');

    if (colonPos != std::string::npos)
    {
        return server.substr(0, colonPos);
    }
    return server; // Retourne la chaîne complète si aucun ':' n'est trouvé
}



std::string e_Methods_to_String(e_Methods method)
{
	if (method == GET) return "GET";
	if (method == DELETE) return "DELETE";
	if (method == POST) return "POST";
	return "UNKNOWN";
}

e_Methods str_to_e_Methods(std::string str)
{
	if (str == "GET") return (GET);
	if (str == "POST") return (POST);
	if (str == "DELETE") return (DELETE);
	return (UNKNOWN);
}

int VerifFatalCallFonc(int ret, std::string msg, bool isFatal)
{
	if (ret < 0)
	{
		if (isFatal)
			Log::log(Log::FATAL, msg.c_str());
		else
			Log::log(Log::ERROR, msg.c_str());
	}
	return ret;
}

void handle_signal(int signal)
{
	if (signal != SIGINT) return;
	std::cout << "\rStopping the server " << std::endl;
	if (fdSocket != -1) close(fdSocket);
	exit(EXIT_SUCCESS);	 // will not cleanup, should be replaced with a flag
}
