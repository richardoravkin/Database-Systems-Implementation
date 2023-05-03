#ifndef _TRIE2_H_
#define _TRIE2_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>


//Relation object into which we load data. Tuples are stored as keys in a map and the multiplicities are the values
typedef struct TrieNode2{
            int value;
            std::vector<TrieNode2*> family; //this is either one parent or many children
            int multiplicity; //only for leaves
            int sizeOfFamily; //only for parents
       
} TrieNode2;

typedef struct less_than{
           bool operator()(const TrieNode2* a,const int &b){
                   return a->value < b;
           };
} less_than;

typedef struct greater_than{
           bool operator()(const int &b, const TrieNode2* a){
                   return a->value > b;
           };
} greater_than;



class Trie2
{
    private:
        std::string Name;
        int numTuples;
        std::vector<TrieNode2*> root; 
        std::vector<TrieNode2*> root2; 
    public:
        Trie2(std::string Name);
        ~Trie2(); 
        void insertTuple(int parent, int child);
        void insertTuple2(int parent, int child);
        int retrieveTuple(int parent, int child);
        void printData();
        void printData2();
        std::vector<TrieNode2*> getRoot();
        std::vector<TrieNode2*> getRoot2();
        int getNum();
};
#endif // _TRIE2_H_