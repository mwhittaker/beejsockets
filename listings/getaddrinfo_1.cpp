#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
int getaddrinfo(
    const char *node,             // e.g. "www.example.com" or IP
    const char *service,          // e.g. "http" or port number
    const struct addrinfo *hints,
    struct addrinfo **res
);
*/

int main() {
    struct addrinfo hints;
    struct addrinfo *servinfo; 

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;   // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags    = AI_PASSIVE;  // fill in my IP for me

    int status = getaddrinfo(NULL, "3490", &hints, &servinfo);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "success!" << std::endl;

    freeaddrinfo(servinfo);
}
