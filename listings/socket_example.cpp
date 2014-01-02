#include <iostream>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
int socket(int domain, int type, int protocol)
*/

int main() {
    struct addrinfo hints;
    struct addrinfo *res; 

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;   
    hints.ai_socktype = SOCK_STREAM; 

    int status = getaddrinfo("www.google.com", "80", &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    std::cout << s << std::endl;

    freeaddrinfo(res);
}
