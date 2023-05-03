#ifndef _QUERYPARSER_H_
#define _QUERYPARSER_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
class QueryParser
{   
    private:
        std::vector<std::string> query;
    
    public:
        QueryParser();
        ~QueryParser();
        std::vector<std::string> getQuery();
        std::pair<std::string,std::vector<std::string>> parseRelation(int i);
        
};
#endif // _QUERYPARSER_