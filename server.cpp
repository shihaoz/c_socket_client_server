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

    int soc_server = tcp_server_bind(_port, AF_INET);
    
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
        
        int data_byte_remain = -1;
        const int _buffer_size = 10;
        char buffer[_buffer_size];
        string msg;
        
        while(1){
            msg.clear();
            if((tcp_receive(soc_request, msg)) == -1){
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
