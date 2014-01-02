#include <iostream>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
*/

int main() {
    struct addrinfo hints;
    struct addrinfo *res; 

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;   // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags    = AI_PASSIVE;  // fill in my IP for me

    int status = getaddrinfo(NULL, "3490", &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s == -1) {
        perror("");
        std::exit(EXIT_FAILURE);
    }

    int err = bind(s, res->ai_addr, res->ai_addrlen);
    if (err == -1) {
        perror("");
        std::exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);
}
