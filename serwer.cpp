#include <iostream>
#include <stdlib.h>
#include <strings.h>
#include "TCPserwer.h"
#include <string>
#include <fstream>
#include <sstream>
#include "BuildResponse.h"






int main() {

    TCPserwer serwer = TCPserwer("0.0.0.0", 8080);
    serwer.startListen();
    return 0;
}
