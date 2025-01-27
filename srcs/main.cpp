#include <cstring>

#include "../simpleserv/basic.cpp" // for testing
#include "Request.hpp"

void test_request_class()
{
    std::string request_content =
        "GET /favicon.ico HTTP/1.1\n"
        "Host: localhost:1234\n"
        "Connection: keep-alive\n"
        "sec-ch-ua-platform: \"Linux\"\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36\n"
        "sec-ch-ua: \"Google Chrome\";v=\"131\", \"Chromium\";v=\"131\", \"Not_A Brand\";v=\"24\"\n"
        "DNT: 1\n"
        "test: test\n"
        "sec-ch-ua-mobile: ?0\n"
        "Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\n"
        "Sec-Fetch-Site: same-origin\n"
        "Sec-Fetch-Mode: no-cors\n"
        "Sec-Fetch-Dest: image\n"
        "Referer: http://localhost:1234/\n"
        "Accept-Encoding: gzip, deflate, br, zstd\n"
        "Accept-Language: en-US,en;q=0.9,fr;q=0.8,pl;q=0.7\n"
        "\n"
        "bodypart 0\n"
        "bodypart 1\n"
        "bodypart 2\n"
        "bodypart 3\n";

    Request request = Request(request_content);

    std::cout << request << std::endl;

    std::cout << request.toString() << std::endl;
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    // simple_server();
    test_request_class();

    return 0;
}
