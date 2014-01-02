#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void usage();
void showip(std::string hostname);

void usage() {
    std::cout << "usage: showip <hostname>" << std::endl;
}

void showip(std::string hostname) {
    struct addrinfo hints;
    struct addrinfo *res;
    char ipstr[INET6_ADDRSTRLEN];
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname.c_str(), NULL, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "IP addresses for " << hostname << std::endl;

    for (struct addrinfo *p = res; NULL != p; p = p->ai_next) {
        if (AF_INET == p->ai_family) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
            struct in_addr     *addr = &(ipv4->sin_addr);
            inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
            std::cout << "IPv4: " << ipstr << std::endl;
        }
        else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            struct in6_addr     *addr = &(ipv6->sin6_addr);
            inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
            std::cout << "IPv6: " << ipstr << std::endl;
        }
    }

    freeaddrinfo(res);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
        std::exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        showip(argv[i]);
        std::cout << ((argc - 1) == i ? "" : "\n");
    } 
}
