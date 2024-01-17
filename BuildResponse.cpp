//
// Created by mdawczak on 25/12/2023.
//

#include "BuildResponse.h"
#include <iostream>
#include <fstream>
#include <string>


std::string BuildResponse(){

        std::ifstream file("questions.txt"); // Open the file for reading

        if (file.is_open()) {

            int lineNumber = 3; // Specify the line number you want to read
            std::string line;

            // Read lines until the desired line is reached
            for (int i = 1; i <= lineNumber; ++i) {
                if (!std::getline(file, line)) {
                    std::cerr << "Reached end of file before reaching the desired line!" << std::endl;
                    // Return an error code
                }
            }

            // Now 'line' contains the string from the desired line
            std::cout << "String from line " << lineNumber << ": " << line << std::endl;

            file.close(); // Close the file
            return line;

        }
        else {
            std::cerr << "Could not open the file!" << std::endl;
            return "qwe"; // Return an error code
        }


    }



