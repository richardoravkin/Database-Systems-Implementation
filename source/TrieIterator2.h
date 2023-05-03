#ifndef _TRIEITERATOR2_H_
#define _TRIEITERATOR2_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Trie2.h"

//This class is responsible of managing which nodes in the trie are passed to leapfrog join


class TrieIterator2
{
    private:
        
        std::string upperVar;
        std::string lowerVar;
        std::vector<TrieNode2*> root;
        
        int hasVal = -1;
        int hasIndex;
        bool equal;
        
        std::vector<TrieNode2*>::iterator upperIt;
        std::vector<TrieNode2*>::iterator lowerIt;
        std::vector<TrieNode2*>::iterator branchIt;
        std::vector<TrieNode2*>::iterator branchEnd;
        std::vector<TrieNode2*>::iterator upperEnd;
        std::vector<TrieNode2*>::iterator lowerEnd;
        
    public:
        TrieIterator2(std::string upperVar,std::string lowerVar,std::string parent, std::vector<TrieNode2*> root);
        TrieIterator2(std::string upperVar,int val, std::vector<TrieNode2*> root, int &status);
        void initializeIterator(std::string var);
        std::pair<int,int> seekKey(std::string var, int key);
        bool updateIterator(std::string var);
        int getCurrent(std::string var);
        int sizeOfFamily();
        bool getEqual();
};
#endif // _TRIEITERATOR2_H_