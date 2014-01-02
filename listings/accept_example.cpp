#include <iostream>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
*/

namespace {
    const char *PORT   = "3490";
    const int  BACKLOG = 10;
}


int main() {
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(sockaddr_storage);
    struct addrinfo hints;
    struct addrinfo *res; 
    int sockfd;
    int new_fd;
    int err;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;   // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags    = AI_PASSIVE;  // fill in my IP for me

    int status = getaddrinfo(NULL, PORT, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("");
        std::exit(EXIT_FAILURE);
    }

    err = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (err == -1) {
        perror("");
        std::exit(EXIT_FAILURE);
    }

    err = listen(sockfd, BACKLOG);
    if (err == -1) {
        perror("");
        std::exit(EXIT_FAILURE);
    }

    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    std::cout << new_fd << std::endl;

    freeaddrinfo(res);
}
