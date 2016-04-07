//
//  socket_tcp.cpp
//  socket_test
//
//  Created by Shihao Zhang on 4/6/16.
//  Copyright © 2016 David Zhang. All rights reserved.
//

#include "socket_tcp.hpp"
/** message is encoded in the format of
 size-message
 */
static int sendall(int p_socket_client, const char* message_encoded, int message_size){
    int total_sent = 0; int byte_send = -1;
    int num_sent = 0;
    while (total_sent < message_size) {
        if ((byte_send = send(p_socket_client, message_encoded, message_size - total_sent, 0)) == -1) {
            perror("send fail");
            return -1;
        }
        total_sent += byte_send;
        cout << ++num_sent << " packaet sent, " << message_size - total_sent << " left\n";
        message_encoded += byte_send;
    }
    
    return 0;
}

int tcp_send(int p_socket_client, string message){
    /* add the size of me ssage, separated by a '-'*/
    string msg = to_string(message.size()) + '-' + message;
    
    if(sendall(p_socket_client, msg.c_str(), msg.size()) == -1){
        return -1;
    }
    return 0;
}

int tcp_receive(int p_socket, string& message, bool confirm){
    int data_byte_remain = -1;
    const int _buffer_size = 10;
    char buffer[_buffer_size];
    bzero(buffer, sizeof(buffer));
    
    data_byte_remain = recv(p_socket, buffer, sizeof(buffer), 0);
    if (data_byte_remain == -1){
        fprintf(stderr, "receive error\n");
        return -1;
    }

    int idx = 0;
    for (; idx < _buffer_size && buffer[idx] != '-'; idx++) {
        message += buffer[idx];
    }idx++;
    /* message size is stored in msg */
    int packet_size = atoi(message.c_str());
    int total_receive = 0;
    
    //cout << "package size: " << packet_size << "\n";
    message.clear();
    message.reserve(packet_size);
    
    do {
        for (; idx < _buffer_size && total_receive < packet_size; idx++) {
            message += buffer[idx];
            total_receive++;
        }
        idx = 0;
    } while (total_receive < packet_size && recv(p_socket, buffer, sizeof(buffer), 0));
    
    if (confirm == true) {
        /* send a confirmation message */
        if(tcp_send(p_socket, to_string(packet_size)+" received") == -1){
            cerr << "error responding to client \n";
            return -1;
        }
    }

    return 0;
}

int tcp_client_connect(string p_host, int p_port, int p_ip_version){
    
    /* set up address, protocol and other stuff */
    struct addrinfo hints, *res, *p;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    
    if ((status = getaddrinfo(p_host.c_str(), to_string(p_port).c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }
    cout << "Target ip address for " << p_host << " -->";
    
    p = res;
    if (p != nullptr) {
        if (p->ai_family == AF_INET) {
            /* ipv4 */
            struct sockaddr_in *ip_addr;
            ip_addr = (struct sockaddr_in*) p->ai_addr;
            char ip_readable[INET6_ADDRSTRLEN];
            inet_ntop(p->ai_family, &ip_addr->sin_addr, ip_readable, sizeof(ip_readable));
            cout << " IPv4: " << ip_readable << "\n";
        }
        else{/* ipv6 */
            struct sockaddr_in6 *ip6_addr;
            ip6_addr = (struct sockaddr_in6*) p->ai_addr;
            char ip_readable[INET6_ADDRSTRLEN];
            inet_ntop(p->ai_family, &ip6_addr->sin6_addr, ip_readable, sizeof(ip_readable));
            cout << " IPv6: " << ip_readable << "\n";
        }
    }
    
    /* create a socket descriptor */
    int socket_client = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    
    /* connect() using the sockfd + address information */
    if ((connect(socket_client, p->ai_addr, p->ai_addrlen)) != 0) {
        fprintf(stderr, "connection failed");
        return -1;
    }
    
    freeaddrinfo(res);
    return socket_client;
}

int tcp_server_bind(int p_port, int p_ip_version){
    struct addrinfo hints, *res, *p;
    int status;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // fill in ip for me
    
    if ((status = getaddrinfo(nullptr, to_string(p_port).c_str(), &hints, &res)) != 0) {
        cerr << status << "\n";
        cerr << "getaddrinfo server: " << gai_strerror(status) << "\n";
        exit(1);
    }
    cout << "IP address for server:";
    
    p = res;
    if (p != nullptr) {
        if (p->ai_family == AF_INET) {
            /* ipv4 */
            struct sockaddr_in *ip_addr;
            ip_addr = (struct sockaddr_in*) p->ai_addr;
            char ip_readable[INET6_ADDRSTRLEN];
            inet_ntop(p->ai_family, &ip_addr->sin_addr, ip_readable, sizeof(ip_readable));
            cout << " IPv4: " << ip_readable << "\n";
        }
        else{/* ipv6 */
            struct sockaddr_in6 *ip6_addr;
            ip6_addr = (struct sockaddr_in6*) p->ai_addr;
            char ip_readable[INET6_ADDRSTRLEN];
            inet_ntop(p->ai_family, &ip6_addr->sin6_addr, ip_readable, sizeof(ip_readable));
            cout << " IPv6: " << ip_readable << "\n";
        }
    }
    
    
    int socket_server = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    int yes = 1;
    if (setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");   exit(1);
    }
    if ((::bind(socket_server, p->ai_addr, p->ai_addrlen)) != 0) {
        cerr << "bind error";
    }
    
    freeaddrinfo(res);
    
    return socket_server;
}


int tcp_info_socket(int p_socket){
    struct sockaddr_storage holder;
    socklen_t size = sizeof(holder);
    
    if ((getsockname(p_socket, (struct sockaddr*)&holder, &size)) == -1) {
        return -1;
    }
    
    switch (holder.ss_family) {
        case AF_INET:
        { /* IP v4 */
            struct sockaddr_in* tmp = (struct sockaddr_in*)&holder;
            char ip_readable[INET_ADDRSTRLEN];
            inet_ntop(p_socket, &tmp->sin_addr, ip_readable, sizeof(ip_readable));
            cout << " socket IPv4: " << ip_readable << "\n";
            cout << "port: " << ntohs(tmp->sin_port) << "\n";
            break;
        }
        case AF_INET6:
        {
            struct sockaddr_in6* tmp = (struct sockaddr_in6*)&holder;
            char ip_readable[INET6_ADDRSTRLEN];
            inet_ntop(p_socket, &tmp->sin6_addr, ip_readable, sizeof(ip_readable));
            cout << " socket IPv6: " << ip_readable << "\n";
            cout << "port: " << ntohs(tmp->sin6_port) << "\n";
            break;
        }
        default:
            return -1;
    }
    return 0;
}


