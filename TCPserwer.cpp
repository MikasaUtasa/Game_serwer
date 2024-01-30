//
// Created by mdawczak on 23/12/2023.
//

/*
struct sockaddr_in
{
    short   sin_family; //must be AF_INET
    u_short sin_port;
    struct  in_addr sin_addr;
    char    sin_zero[8]; // Not used, must be zero //
};
*/



#include "TCPserwer.h"
#include <string>
#include <cstdio>
#include <sys/socket.h>
#include <iostream>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <thread>
#include <future>
#include <pthread.h>
#include "BuildResponse.h"
#include <vector>
#include <mutex>
#include "BuildResponse.h"



TCPserwer::TCPserwer(std::string srv_ip_addr ,int port) {
    TCPserwer::logging_file = "logs.txt";
    TCPserwer::result_file_name = "results.txt";
    TCPserwer::port = port;
    TCPserwer::srv_ip_address = srv_ip_addr;
    TCPserwer::points = 0;
    startSerwer();
    //startListen();
}

TCPserwer::~TCPserwer() {
    cleanUpThreads();
    stopSerwer();
}


void TCPserwer::log(const std::string &message, int type) {
    time_t tmNow;
    tmNow = time(NULL);
    struct tm t = *localtime(&tmNow);
    log_file << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << "  ";

    switch (type) {
        case 0:
            log_file << "INFO[0] ";
            std::cout << "INFO[0] ";
            break;
        case 1:
            log_file << "ERR[1] ";
            std::cout << "ERR[1] ";
            break;
    }
    log_file << message << std::endl;
    std::cout << message << std::endl;
}


void TCPserwer::result(const std::string &message) {
    result_file << message << std::endl;
}


int TCPserwer::startSerwer() {
    bzero((char *) &serv_addr, sizeof(serv_addr)); //Zerowanie buforu
    TCPserwer::log_file.open(TCPserwer::logging_file);
    TCPserwer::result_file.open(TCPserwer::result_file_name);
    //result_file << "wyniki" << std::endl;
    std::ostringstream sa;
    std::ostringstream sb;
    sb << "Wyniki Graczy" << std::endl;
    sa << "Serwer start Logging" << std::endl;
    TCPserwer::log(sa.str(), 0);
    TCPserwer::result(sb.str());


    sockfd = socket(AF_INET, SOCK_STREAM, 0); //Tworzenie gniazda
    //setsockopt(sockfd ,SO_REUSEADDR, 1);
    if (sockfd < 0) //Nieudane zgniazdo zwraca -1
        log("ERROR opening socket", 1);

    TCPserwer::serv_addr.sin_family = AF_INET; //Przypisanie
    TCPserwer::serv_addr.sin_port = htons(port);
    TCPserwer::serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        log("Bind ERROR", 1);
        exit(-1);
        return 1;
    } //Nieudany bind zwraca -1

    //TCPserwer::startListen();
    return 0;
}

void TCPserwer::stopSerwer() {
    close(sockfd);
    close(newsockfd);
    TCPserwer::log_file.close();
    TCPserwer::result_file.close();
    exit(0);
}

void TCPserwer::startListen() {
    if (listen(sockfd, 5) < 0) {
        log("Socket listen failed", 1);
    }
    std::ostringstream ss;
    ss << "*** Listening on address: "
       << inet_ntoa(serv_addr.sin_addr)
       << " PORT: " << ntohs(serv_addr.sin_port)
       << " ***\n";
    log(ss.str(), 0);

    while (true) {
        sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(sockfd, (sockaddr*)&clientAddress, &clientAddressSize);


        if (clientSocket == -1) {
            ss << "Failed to accept connection from "
               << inet_ntoa(clientAddress.sin_addr)
               << ":"
               << ntohs(clientAddress.sin_port);
            log(ss.str(), 1);
        }

        ss.clear();
        ss << "Connection from "
           << inet_ntoa(clientAddress.sin_addr)
           << ":"
           << ntohs(clientAddress.sin_port);
        log(ss.str(), 0);
        ss.str("");

        std::thread connectionThread([this, clientSocket, clientAddress]() {
            sockaddr_in copyOfClientAddress = clientAddress;
            this->handleConnection(clientSocket, copyOfClientAddress);
        });
        Threads.emplace_back(std::move(connectionThread));
    }

}


int TCPserwer::acceptConnection() {
    clilen = sizeof(cli_addr);
    std::ostringstream ss;
    sockaddr_in client = cli_addr;
    int new_sock = accept(sockfd, (sockaddr*) &cli_addr, &clilen);
    if (newsockfd < 0) {
        ss << "Failed to accept connection from "
           << inet_ntoa(cli_addr.sin_addr)
           << ":"
           << ntohs(cli_addr.sin_port);
        log(ss.str(), 1);
        return -1;
    }
    ss.str("");
    ss << "Connection from "
       << inet_ntoa(cli_addr.sin_addr)
       << ":"
       << ntohs(cli_addr.sin_port);
    log(ss.str(), 0);
    handleConnection(new_sock, client);
    return 1;
}

void TCPserwer::handleConnection(/*void *new_sockfd*/int new_sock, sockaddr_in &client) {

    std::string hello_msg = "Hello from the server";
    sendData(hello_msg, new_sock);
    //std::ifstream file("results.txt"); // Open the file for reading

    std::ostringstream ss1;
    std::ostringstream ss3;

    ss1 << ntohs(client.sin_port);
    std::string ss2 = ss1.str();
     ss3 << "User "
                << ss2
                << ": ";
    sendData(ss2 , new_sock);
    while (new_sock >= 0) {

        char buffer[BUFFER_SIZE] = {0};
        std::ostringstream ss;
        std::ostringstream client_str;
        client_str << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port);
        std::string buff;// = "Hello from serwer";
        //sendData(buff);
        rcv = read(new_sock, buffer, BUFFER_SIZE);
        std::cout << buffer << std::endl;
        if (rcv <= 0) {
            log("Failed to read bytes from client socket connection", 0);
            close(new_sock);
            break;
        }
        int recieved_points = atoi(buffer);
        TCPserwer::points += recieved_points;
        //ss.str(std::string());
        ss << "------ Received data from " << client_str.str() << "   :" << buffer;
        log(ss.str(), 0);

        for(int i = 0; i < 5; i++){

            buff = BuildResponse();
            sendData(buff, new_sock);
            rcv = read(new_sock, buffer, BUFFER_SIZE);
            ss.str("");
            ss << "------ Received data from " << client_str.str() << "   :" << buffer;
            log(ss.str(), 0);
            //int recieved_points = atoi(buffer);
            //std::cout << recieved_points << std::endl;

        }
        buff = "END";
        sendData(buff, new_sock);
        rcv = read(new_sock, buffer, BUFFER_SIZE);
        if (rcv <= 0) {
            log("Failed to read bytes from client socket connection", 0);
            close(new_sock);
            //TCPserwer::cleanUpThreads();
            break;
        }
        ss3 << buffer;
        result(ss3.str());
        result("\n");
        //std::cout << "gowno" << buffer << std::endl;
        buff = buffer;
        sendData(buff, new_sock);
        std::ifstream file ("results.txt");
        ss.str("");
        ss << file.rdbuf();
        file.close();
        std::string koniec = ss.str();
        //std::cout << koniec << std::endl;
        sendData(koniec, new_sock);
    }
    //close(new_sock);
}

void TCPserwer::sendData(std::string &buffer, int &socket) {
    long bytesSent;
    std::ostringstream ss;
    bytesSent = write(socket, buffer.c_str(), buffer.size());
    if (bytesSent == buffer.size())
    {
        ss << "------ Server Response sent to client:   " << buffer;
        log(ss.str(), 0);
        //log("------ Server Response sent to client:");
    }
    else
    {
        log("Error sending response to client", 1);
    }
}


void TCPserwer::createThread() {

    //TCPserwer::threads.emplace_back(&TCPserwer::handleConnection, this, new_sock);
    //TCPserwer::threads.back().detach();
    std::thread t1([this]() {
        acceptConnection();
    });
    std::thread t2([this]() {
        acceptConnection();
    });
    std::cout << t1.get_id() << std::endl;
    std::cout << t2.get_id() << std::endl;

    t1.join();
    t2.join();

}

void TCPserwer::cleanUpThreads() {
    std::lock_guard<std::mutex> lock(std::mutex);

    // Remove finished threads from the list
    Threads.erase(
            std::remove_if(Threads.begin(), Threads.end(),
                           [](const std::thread& t) { return t.joinable(); }),
            Threads.end());
}





