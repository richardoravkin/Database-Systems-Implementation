#ifndef _QUERYRESULT_H_
#define _QUERYRESULT_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>

//this class builds objects of type {x->1,y->2, z -> 3} (along with multiplicities) in a way that makes it easy to (Wander) join them

class QueryResult
{
    private:
        std::vector<std::string> varNames;
        //we store the data as a vector of pairs [(x,1),(y,2),(z,3)]. This is mapped to its multiplicity. 
        typedef std::map<std::vector<std::pair<std::string,int>>, int> QData;
        int numOfTuples;
        QData Data; 

    public:
        QueryResult(std::vector<std::string> varNames);
        ~QueryResult();
        void insertData(std::vector<std::pair<std::string,int>> data, int multiplicity);
        void printData();
        std::vector<std::string> getvarNames();
        QData getData();
        int getNumTuples();
};
#endif // _QUERYRESULT_H_