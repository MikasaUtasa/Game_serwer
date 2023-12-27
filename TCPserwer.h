//
// Created by mdawczak on 23/12/2023.
//
#ifndef SERWER_TCPSERWER_H
#define SERWER_TCPSERWER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>




class TCPserwer {
public:
    TCPserwer(std::string srv_ip_addr ,int port, std::string logging_file);
    ~TCPserwer();
    void startListen();
    const int BUFFER_SIZE = 30720;

private:
    std::string srv_ip_address;
    std::string logging_file;
    std::ofstream log_file;
    std::string serwerMessage;

    long message;
    int sockfd, newsockfd, port, rcv;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];

    void log(const std::string &message, int type);
    int startSerwer();
    void stopSerwer();
    int acceptConnection();
    void handleConnection(/*void *new_sockfd*/ int new_sock);
    void sendData(std::string  &buffer);
    void createThread();
};


#endif //SERWER_TCPSERWER_H
