#ifndef _LEVEL2_H_
#define _LEVEL2_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include "Trie2.h"
#include "TrieIterator2.h"



class Level2
{
    private:
        std::vector<TrieIterator2*> Iterators;
        std::string variable;
        std::set<std::string> relatedVars;
        std::vector<std::pair<int,int>> cache;
        std::vector<std::vector<int>> iteratorCache;
        int LeapFrogKey;
        bool equal = false; //special case of R(x,x)
    public:
        Level2(std::vector<TrieIterator2*> Iterators, std::string variable);
        ~Level2();
        void initialize();
        void update();
        std::pair<int,int> emit();
        std::string getVariable();
        void initializeKey();
        int num();
        int sizeOfFamily();
        bool getEqual();
};
#endif // _LEVEL2_H_