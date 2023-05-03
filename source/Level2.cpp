#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include "Trie2.h"
#include "TrieIterator2.h"
#include "Level2.h"

Level2::Level2(std::vector<TrieIterator2*> Iterators, std::string variable){
    this->Iterators = Iterators;
    this->variable = variable;
    if (Iterators.size() == 1){
        if (Iterators[0]->getEqual()){
            this->equal = true;
        }
    }

}

std::pair<int,int> Level2::emit(){
    //perform leapfrog join accross iterators linked to this->variable. If found, take product of multiplicities. 
    int match = 0;
    int mult = 1;
    
    while (true){ //we compute leapfrog join until iterators finished or we find match
        for (int i = 0; i < this->Iterators.size(); i++){
            //iterate over iterators and seek
            std::pair<int,int> temp;
            temp = this->Iterators[i]->seekKey(this->variable,this->LeapFrogKey);
            if (temp.first != - 1){
                if (temp.first == this->LeapFrogKey){
                    match += 1; //we found a match
                    mult *= temp.second;
                    if (match == this->Iterators.size()){
                        //all iterators match, level can emit.
                        std::pair<int,int> res; 
                        res.first = this->LeapFrogKey;
                        res.second = mult;
                        this->LeapFrogKey += 1; //we manually increment the key otherwise we would keep emitting it
                        return res;
                    }
                }else{
                    //some iterator didn't match, we must seek next value
                    match = 0;
                    mult = 1;
                    this->LeapFrogKey = temp.first;
                }
            }else{
                //some iterator has reached end and nothing was emitted. We are done.
                std::pair<int,int> res;
                res.first = -1;
                res.second = -1;
                return res; 
            }
        }
    } 
}

void Level2::initialize(){
    //reposition iterators on right place. 
    for (int i = 0; i < this->Iterators.size(); i++){
        this->Iterators[i]->initializeIterator(this->variable);
    }
}

std::string Level2::getVariable(){
    return this->variable;
}

void Level2::initializeKey(){
    //initializes leapfrog key to the largest initial value from all iterators
    this->LeapFrogKey = -1;
    for (int i = 0; i < this->Iterators.size(); i++){
        if (this->Iterators[i]->getCurrent(variable) > this->LeapFrogKey){
            this->LeapFrogKey = this->Iterators[i]->getCurrent(variable);
            
        }
    }
}

int Level2::num(){
    return this->Iterators.size();
}
int Level2::sizeOfFamily(){
    return this->Iterators[0]->sizeOfFamily();
}

bool Level2::getEqual(){
    return this->equal;
}


