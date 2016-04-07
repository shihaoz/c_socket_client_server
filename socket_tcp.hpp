//
//  socket_tcp.hpp
//  socket_test
//
//  Created by Shihao Zhang on 4/6/16.
//  Copyright © 2016 David Zhang. All rights reserved.
//

#ifndef socket_tcp_hpp
#define socket_tcp_hpp


#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>


using namespace std;

/**
 @param target host name, target port, ip version (AF_INET or AF_INET6)

 @return
 success: socket descriptor
 failure: -1
 
 @description
 creates a socket that connects to target host via port. 
 ready to send()/recv()
 */
int tcp_client_connect(string p_host, int p_port, int p_ip_version);

/** 
 @param ip version, port number

 @return
 success: socket descriptor
 failure: -1
 
 @description
 creates a socket for tcp connection, bind it to local ip address + port number
 ready to listen()
 */
int tcp_server_bind(int p_port, int p_ip_version=AF_INET);


/**
 @param an established socket, an empty message
 @return 
 success: 0
 failure: -1
 */
int tcp_receive(int p_socket, string& message);
/**
 @param  an established socket, message
 @return
 success: 0 
 failure: -1
 */
int tcp_send(int p_socket, string message);
#endif /* socket_tcp_hpp */
