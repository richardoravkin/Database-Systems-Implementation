#include "QueryParser.h" 
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

//Parser. Not great not terrible. 
QueryParser::QueryParser(){
    
    std::string input;
   
    std::getline(std::cin, input);
    std::stringstream ss(input);
    std::string word;
    while (getline(ss, word, ' ') || getline(ss, word, ',')) { //Separates by space or comma. 
        query.push_back(word);
    }
}


std::vector<std::string> QueryParser::getQuery(){
    return this->query;
}


std::pair<std::string,std::vector<std::string>> QueryParser::parseRelation(int i){
    std::string relname = "";
    std::vector<std::string> datapoint;

    std::string token1;
    std::istringstream stream(this->query[i]); 
    int j = 0;
    int pos1 = -1;
    int pos2 = -1;
    for (int j = 0; j < this->query[i].length(); j ++){
        if (this->query[i][j] == '('){
            pos1 = j;
        }else if (this->query[i][j] == ')'){
            pos2 = j;
        }
    }
    if (pos1 == -1 || pos2 == -1){
        std::cerr << "Invalid syntax - check brackets" << std::endl;
    } else{
        relname = this->query[i].substr(0,pos1);
        
        std::string token = this->query[i].substr(pos1+1,pos2-pos1-1);
        size_t pos = 0;
        //int counter = 0;
        while ((pos = token.find(',')) != std::string::npos){
            //counter += 1;
            datapoint.push_back(token.substr(0,pos));
            token = token.substr(pos+1);
        }
        datapoint.push_back(token);
    }
    std::pair<std::string,std::vector<std::string>> x;
    x.first = relname;
    x.second = datapoint;
    return x;
}