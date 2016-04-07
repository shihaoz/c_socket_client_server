//
//  main.cpp
//  socket_test
//
//  Created by Shihao Zhang on 4/5/16.
//  Copyright © 2016 David Zhang. All rights reserved.
//
#include "socket_tcp.hpp"
using namespace std;

string _port = "3490";

int main(int argc, const char * argv[]) {
    
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];
    if (argc != 3) {
        fprintf(stderr,"usage: showip hostname\n");
        return 1;
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    
    _port = argv[2];
    
    if ((status = getaddrinfo("localhost", _port.c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    printf("Target IP addresses for %s:\n", argv[1]);
    
    p = res;

    /* addrinfo *p is important !! */
    int soc_client = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    
    if((connect(soc_client, res->ai_addr, res->ai_addrlen)) == -1){
        fprintf(stderr, "connection failed");
    }
    
    tcp_info_socket(soc_client);
    
    string msg;
    cout << "me: " << "\n";
    while (getline(cin, msg)) {
        /* send the message, use endline as deliminator */
        if (msg.empty()) {
            continue;
        }
        if((tcp_send(soc_client, msg)) == -1){
            cerr << "send failed";
            exit(1);
        }

        msg.clear();
        if ((tcp_receive(soc_client, msg, false)) == -1) {
            cerr << "confirmation received fail";
            exit(1);
        }
        cout << "bot: \n" << msg << "\n";
        cout.flush();
    }

    
    close(soc_client);
    return 0;
}
