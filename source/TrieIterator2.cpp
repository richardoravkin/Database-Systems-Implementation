#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Trie2.h"
#include "TrieIterator2.h"
//case of two integers is ignored because is trivial - do it later
TrieIterator2::TrieIterator2(std::string upperVar,std::string lowerVar,std::string parent, std::vector<TrieNode2*> root){
    this->root = root;
    this->upperVar = upperVar; //the variable which is linked to the top level of the trie
    this->lowerVar = lowerVar; //the variable which is linked to the bottom level of the trie
    if (upperVar == lowerVar){
        this->equal = true; //corresponds to queries of type R(x,x)
    }
}
TrieIterator2::TrieIterator2(std::string lowerVar,int val, std::vector<TrieNode2*> root, int &status){
    //case when relation of type R(2,x) or R(x,2)
    this->root = root; //the root of the right trie
    this->lowerVar = lowerVar; //always chosen as lower var
    this->hasVal = val; 
    
    auto lower = std::lower_bound(root.begin(), root.end(), val, less_than()); //check whether the value exists on the top level
    const bool found = lower != root.end() && (*lower)->value == val;
    if (found){
        this->hasIndex = lower - root.begin();
    }else{
        //this happens if user inputs R(const,x) but the const does not exists on that level. Automatically zero. 
        status = -1;
        //exit(1);  
    }
}

void TrieIterator2::initializeIterator(std::string var){
   //IMPORTANT: This gets called by Levels. This ensures that the "bottom" is always positioned on the child nodes of the "top" iterator
   //we know the top iterator always comes first in the order, because that is how we choose the right trie. 
    if (var == this->upperVar){
        this->upperIt = this->root.begin();
        this->upperEnd = this->root.end();
        
    }else if(var == this->lowerVar){
        if (this->hasVal == -1){
            this->lowerIt = (*this->upperIt)->family.begin();
            this->lowerEnd = (*this->upperIt)->family.end();
        }else{
            this->lowerIt = this->root[this->hasIndex]->family.begin();
            this->lowerEnd = this->root[this->hasIndex]->family.end();
        }
    }
}

std::pair<int,int> TrieIterator2::seekKey(std::string var, int key){
    // perform leapfrog step on the iterator linked to the variable var. This function gets called by Level2 during leapfrog. 
    std::pair<int,int> temp;
    if (var == this->upperVar){
        if (this->equal){ //this corresponds to queries of type R(x,x)
            //We must check upper and lower simultaneously
            while (true){
                //search for key on upper level
                this->upperIt = std::lower_bound(this->upperIt, this->upperEnd, key, less_than());
                const bool found = this->upperIt != this->upperEnd; 
                //reposition lower iterator
                if (found){
                    int val = (*this->upperIt)->value;
                    this->lowerIt = (*this->upperIt)->family.begin();
                    this->lowerEnd = (*this->upperIt)->family.end();
                    //search for val (not key!) in lower branch
                    this->lowerIt = std::lower_bound(this->lowerIt, this->lowerEnd, val, less_than());
                    const bool found2 = this->lowerIt != this->lowerEnd && (*this->lowerIt)->value ==val ; //values of iterators must be equal!
                    if (found2){
                        temp.first = (*this->lowerIt)->value;
                        temp.second = (*this->lowerIt)->multiplicity;
                        return temp;
                    }else{
                        key += 1; //if we didnt find it, we look for next key 
                    }
                }else{
                    temp.first = -1;
                    temp.second = -1;
                    return temp;
                }
            }
            
        }else{
            //in the "normal" case, leapfrog is just std::lower bound
            this->upperIt = std::lower_bound(this->upperIt, this->upperEnd, key, less_than());
            const bool found = this->upperIt != this->upperEnd; 
            
            if (found){
                temp.first = (*this->upperIt)->value;
                temp.second = (*this->upperIt)->multiplicity; //this is always 1 since we are on parent level
                return temp;
            }else{
                temp.first = -1; //the iterator has reached its end, meaning the entire level will fail to emit.
                temp.second = -1;
                return temp;
            }
        }
    } else if(var == this->lowerVar && !this->equal){
        this->lowerIt = std::lower_bound(this->lowerIt, this->lowerEnd, key, less_than());
        const bool found = this->lowerIt != this->lowerEnd; 
        if (found){
            temp.first = (*this->lowerIt)->value;
            temp.second = (*this->lowerIt)->multiplicity;
            return temp;
        }else{
            temp.first = -1;
            temp.second = -1;
            return temp;
        }
    }
}
    
int TrieIterator2::getCurrent(std::string var){
    if (this->upperVar == var){
        return (*this->upperIt)->value; 
    }else if(this->lowerVar == var){
        return (*this->lowerIt)->value;
    }
}

int TrieIterator2::sizeOfFamily(){
    if (this->hasVal == -1){
        return (*this->upperIt)->sizeOfFamily;
    }else{
        return this->root[this->hasIndex]->sizeOfFamily;
    }
}
bool TrieIterator2::getEqual(){
    return this->equal;
}