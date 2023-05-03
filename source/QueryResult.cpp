#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "QueryResult.h"


//some auxillary stuff for printing results. 
typedef std::map<std::vector<std::pair<std::string,int>>, int> QData;

QueryResult::QueryResult(std::vector<std::string> varNames){
    this->varNames = varNames;
}
int QueryResult::getNumTuples(){
    return this->numOfTuples;
}

void QueryResult::insertData(std::vector<std::pair<std::string,int>> data, int multiplicity){
    this->numOfTuples += multiplicity;
    if (this->Data.find(data) == this->Data.end()) {
        this->Data[data] = multiplicity;
    } else {
        this->Data[data] += multiplicity;
    } 
}

void QueryResult::printData(){
    std::map<std::vector<std::pair<std::string,int>>, int>::iterator it;
    std::cout << "printing" <<std::endl;
    for (it = this->Data.begin(); it != this->Data.end(); it++)
    {
        for (int j = 0; j < it->first.size(); j ++){
            std::cout<<it->first[j].first << "-->" << it->first[j].second << " ";
        }
        std::cout << " with multiplicity "<< it->second << std::endl;
    }
}

std::vector<std::string> QueryResult::getvarNames(){
    return this->varNames;
}

QData QueryResult::getData(){
    return this->Data;
};