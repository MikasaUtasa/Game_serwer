//
// Created by mdawczak on 25/12/2023.
//

#include "BuildResponse.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>


std::string BuildResponse(){

    std::vector<std::vector<std::string>> quizData = {
            {"Question 1", "Answer A", "Answer B", "Answer C", "A"},
            {"Question 2", "Answer A", "Answer B", "Answer C", "A"},
            {"Question 3", "Answer A", "Answer B", "Answer C", "A"},
            {"Question 4", "Answer A", "Answer B", "Answer C", "A"},
            {"Question 5", "Answer A", "Answer B", "Answer C", "A"}
    };
    srand((unsigned) time(0));
    int random = rand()%4;
    std::vector question = quizData[random];
    std::string line;
    for(int i = 0; i< question.size(); i++){
        line.append(question[i]);
        line.append("::");
    }
    return line;
}



