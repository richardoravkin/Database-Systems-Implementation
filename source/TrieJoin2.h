#ifndef _TRIEJOIN2_H_
#define _TRIEJOIN2_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Trie2.h"
#include "TrieIterator2.h"
#include "QueryResult.h"
#include "Level2.h"

//this class is responsible for executing the TrieJoin, using LeapFrogJoin and information from the TrieIterator class
typedef struct cmp{
        bool operator()(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        }
    }cmp;


class TrieJoin2
{
    private:
        std::vector<std::vector<TrieNode2*>> roots;
        std::vector<std::string> vars;
        //std::vector<TrieIterator*> trieIterators;
        std::map<std::string,std::vector<TrieIterator2*>> varMap;
        bool emptyJoin = false;
        
    public:
        TrieJoin2(std::vector<std::vector<TrieNode2*>> roots,std::vector<std::vector<TrieNode2*>> roots2, std::vector<std::vector<std::string>> bindingVars);
        std::pair<bool,bool> checkInput(std::vector<std::string> pair);
        std::pair<bool,std::pair<std::string,std::string>> getOrdered(std::vector<std::string> pair,std::vector<std::string> order);
        int joinQueries();
        std::vector<std::string> generateOrder(std::vector<std::vector<std::string>> bindingVars);
};

#endif // _TRIEJOIN_H2_