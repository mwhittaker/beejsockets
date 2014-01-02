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
    const char *PORT   = "3490";
    const int  BACKLOG = 10;
}

void sigchld_handler(int) {
    while(waitpid(-1, NULL, WNOHANG));
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    else {
        return &(((struct sockaddr_in6 *)sa)->sin6_addr);
    }
}

int main() {
    int sockfd;                         // listen on sock_fd
    int new_fd;                         // new connection on new_fd
    struct addrinfo hints;              // address information hints
    struct addrinfo *servinfo;          // linked list of addrinfos
    struct sockaddr_storage their_addr; // connector's address information
    struct sigaction sa;                // magical wizardry
    int yes = 1;                        // ibid.
    char s[INET6_ADDRSTRLEN];           // address string
    int rv;                             // return value, used for error checking

    /* Initialize hints */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    /* getaddrinfo */
    rv = getaddrinfo(NULL, PORT, &hints, &servinfo);
    if (rv != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        std::exit(EXIT_FAILURE);    
    }  

    /* socket and bind */
    struct addrinfo *p;
    for (p = servinfo; NULL != p; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (-1 == sockfd) {
            perror("server: socket");
            continue;
        }

        rv = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (rv == -1) {
            perror("setsockopt");
            std::exit(EXIT_FAILURE);
        }

        rv = bind(sockfd, p->ai_addr, p->ai_addrlen);
        if (rv == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }
    
    if (NULL == p) {
        std::cerr << "server: failed to bind" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    /* listen */
    rv = listen(sockfd, BACKLOG);
    if (rv == -1) {
        perror("listen");
        std::exit(EXIT_FAILURE);
    }

    /* magic guru signal stuff */
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        std::exit(EXIT_FAILURE);
    }

    /* accept */
    std::cout << "server: waiting for connections..." << std::endl;

    while(true) {
        socklen_t sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
       
        void *saddr = get_in_addr((struct sockaddr *)&their_addr);
        inet_ntop(their_addr.ss_family, saddr, s, sizeof(s));
        std::cout << "server: got connection from " << s << std::endl;

        if (send(new_fd, "Hello, world!", 13, 0) == -1) {
            perror("send");
        }
        close(new_fd); 
    }
}

