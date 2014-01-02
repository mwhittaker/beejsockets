#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

namespace {
    const char *PORT        = "3490";
    const int   MAXDATASIZE = 100;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    else {
        return &(((struct sockaddr_in6 *)sa)->sin6_addr);
    }
}

int main(int argc, char *argv[]) {
    int sockfd;
    int numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int rv;
    char s[INET6_ADDRSTRLEN];

    /* command line args */
    if (argc != 2) {
        std::cerr << "usage: client <hostname>" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    /* getaddrinfo */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    rv = getaddrinfo(argv[1], PORT, &hints, &servinfo);
    if (rv != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    /* connect */
    struct addrinfo *p;
    for (p = servinfo; NULL != p; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (-1 == sockfd) {
            perror("client: socket");
            continue;
        }

        rv = connect(sockfd, p->ai_addr, p->ai_addrlen);
        if (rv == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        
        break;
    }

    if (NULL == p) {
        std::cerr << "client: failed to connect" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    void *saddr = get_in_addr((struct sockaddr *)p->ai_addr);
    inet_ntop(p->ai_family, saddr, s, sizeof(s));
    std::cout << "client connecting to " << s << std::endl;

    freeaddrinfo(servinfo);

    /* recv */
    numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);
    if (numbytes == -1) {
        perror("recv");
        std::exit(EXIT_FAILURE);
    }

    buf[numbytes] = '\0';
    std::cout << "client received: " << buf << std::endl;
    close(sockfd);
}
