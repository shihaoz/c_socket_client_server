//
//  main.cpp
//  socket_test
//
//  Created by Shihao Zhang on 4/5/16.
//  Copyright © 2016 David Zhang. All rights reserved.
//
#include "socket_tcp.hpp"
using namespace std;

const string _port = "3490";

int main(int argc, const char * argv[]) {
    
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];
    if (argc != 2) {
        fprintf(stderr,"usage: showip hostname\n");
        return 1;
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    
    if ((status = getaddrinfo("localhost", _port.c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    printf("Target IP addresses for %s:\n\n", argv[1]);
    
    p = res;
    if (p != nullptr) {
        if (p->ai_family == AF_INET) {
            /* ipv4 */
            struct sockaddr_in *ip_addr;
            ip_addr = (struct sockaddr_in*) p->ai_addr;
            char ip_readable[INET6_ADDRSTRLEN];
            inet_ntop(p->ai_family, &ip_addr->sin_addr, ip_readable, sizeof(ip_readable));
            cout << ":: IPv4: " << ip_readable << "\n";
        }
        else{/* ipv6 */
            struct sockaddr_in6 *ip6_addr;
            ip6_addr = (struct sockaddr_in6*) p->ai_addr;
            char ip_readable[INET6_ADDRSTRLEN];
            inet_ntop(p->ai_family, &ip6_addr->sin6_addr, ip_readable, sizeof(ip_readable));
            cout << ":: IPv6: " << ip_readable << "\n";
        }
    }
    
    /* addrinfo *p is important !! */
    int soc_client = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    
    if((connect(soc_client, res->ai_addr, res->ai_addrlen)) == -1){
        fprintf(stderr, "connection failed");
    }
    
    string msg;
    while (getline(cin, msg)) {
        /* send the message, use endline as deliminator */
        if (msg.empty()) {
            continue;
        }
        tcp_send(soc_client, msg);
    }

    
    close(soc_client);
    return 0;
}
