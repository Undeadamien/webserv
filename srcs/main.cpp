#include <cstring>

#include "../simpleserv/basic.cpp" // for testing

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    simple_server();
    return 0;
}
