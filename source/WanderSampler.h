#ifndef _WANDERSAMPLER_H_
#define _WANDERSAMPLER_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Trie2.h"
#include "JoinOrder.h"




//Relation object into which we load data. Tuples are stored as keys in a map and the multiplicities are the values

class WanderSampler
{
    private:
        std::string var1;
        std::string var2;
        int hasVal = -1;
        std::vector<TrieNode2*> root;
        bool equal;
        

    public:
        WanderSampler(std::string var1, std::string var2, std::vector<TrieNode2*> root);
        WanderSampler(std::string var1, int val, std::vector<TrieNode2*> root);
        ~WanderSampler();
        double sample(std::map<std::string,int>* fixedVars);
        double sample2(std::map<std::string,int>* fixedVars);
};
#endif // _WANDERSAMPLER_H_