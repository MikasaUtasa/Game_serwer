#include <iostream>
#include <stdlib.h>
#include <strings.h>
#include "TCPserwer.h"
#include <string>
#include <fstream>
#include <sstream>
#include "BuildResponse.h"






int main() {


    std::string b = "qweqweqwewqewqe";
    std::string a = BuildResponse();
    b.append(a);

    std::cout << b << std::endl;
    TCPserwer serwer = TCPserwer("0.0.0.0", 8080);
    serwer.startListen();

    //

    //listen(sockfd, 5);
    //clilen = sizeof(cli_addr);

    //newsockfd = socket()




    return 0;
}
