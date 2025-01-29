#ifndef ARGUMENTS_HPP

# define ARGUMENTS_HPP

# include <iostream>


bool check_arguments(int argc, char **argv);
bool filename_parser(const std::string &filename);
bool check_options(int argc, char **argv);

#endif
