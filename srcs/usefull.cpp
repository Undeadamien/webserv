#include "usefull.hpp"

std::string itos(int value)
{
    std::stringstream ss;
    ss << value;
    return (ss.str());
}
