#include "usefull.hpp"

#include "Log.hpp"

std::string itos(int value)
{
	std::stringstream ss;
	ss << value;
	return (ss.str());
}


