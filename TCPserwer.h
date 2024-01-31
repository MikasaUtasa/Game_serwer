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
#include <vector>
#include <thread>



class TCPserwer {
public:
    TCPserwer(std::string srv_ip_addr ,int port);
    ~TCPserwer();
    void startListen();
    static const int BUFFER_SIZE = 30720;
    void log(const std::string &message, int type);
    void result(const std::string &message);

private:
    std::string srv_ip_address;
    std::string logging_file;
    std::ofstream log_file;
    std::string result_file_name;
    std::ofstream result_file;
    std::string serwerMessage;
    std::vector<std::thread> Threads;
    int points;

    long message;
    int sockfd, port;
    int rcv, newsockfd;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];


    int startSerwer();
    void stopSerwer();
    int acceptConnection();
    void handleConnection(/*void *new_sockfd*/ int new_sock, sockaddr_in &client);
    void sendData(std::string  &buffer, int &socket);
    void createThread();
    void cleanUpThreads();
};

#endif //SERWER_TCPSERWER_H
