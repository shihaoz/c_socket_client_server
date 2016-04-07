//
//  main.cpp
//  socket_test
//
//  Created by Shihao Zhang on 4/5/16.
//  Copyright © 2016 David Zhang. All rights reserved.
//

#include "socket_tcp.hpp"
using namespace std;

const int _port = 3490;

int main(int argc, const char * argv[]) {

    int soc_server = tcp_server_bind(0);
    
    struct sockaddr_in local;
    int local_length = sizeof(local);
    memset(&local, 0, local_length);
    
    if ((getsockname(soc_server, (struct sockaddr*)&local, (socklen_t*)&local_length)) == -1) {
        perror("get socket name failed");
        exit(1);
    }
    char hostname[INET_ADDRSTRLEN];
    socklen_t hostname_len = INET_ADDRSTRLEN;
    inet_ntop(AF_INET, &(local.sin_addr), hostname, hostname_len);
    cout << "server IP: "<< hostname << "\n";
    cout << "port: " << ntohs(local.sin_port) << "\n";
    
    /* a server must 1. listen() 2. accept() */
    if (listen(soc_server, 2) != 0){
        fprintf(stderr, "listen error\n");        
    }    

    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);

    int soc_request;
    while (soc_request = accept(soc_server, (struct sockaddr *)&their_addr, &addr_size)) {
        
        /* accept extracts the first pending request on the waiting queue of listening socket
         if none present, it will block (unless non-blocking is specified) */
        
        string msg;
        
        while(1){
            msg.clear();
            if((tcp_receive(soc_request, msg, true)) == -1){
                cerr << "receive failed, connection ending\n";
                exit(1);
            }
            cout << msg << "\n";
        }// end-while
        
        close(soc_request);
    }

    
    close(soc_server);
    return 0;
}
