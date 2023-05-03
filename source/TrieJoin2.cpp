#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include "Trie2.h"
#include "TrieIterator2.h"
#include "TrieJoin2.h"
#include "QueryResult.h"
#include "Level2.h"
#include "JoinOrder.h" 

bool test = true; //change this to true for printing
bool wantSort = false; //change this to true for the join "optimization" - terrible idea

TrieJoin2::TrieJoin2(std::vector<std::vector<TrieNode2*>> roots,std::vector<std::vector<TrieNode2*>> roots2,std::vector<std::vector<std::string>> bindingVars){
    //This is where we prepare our tries and decide how to setup our iterators
    //for each relation inputted in the query, we check what the "binding variables" are. Namely, are they integers or variables?
    //This matters. For example,  if the user inputted R(2,x), we know exactly where to open iterator on the x variable. 

    std::map<std::string, int> temp;
    std::pair<bool,std::vector<std::vector<std::string>>> ret = joinOrder(bindingVars,temp, 0, bindingVars.size()-1); //this check whether the join is 0 by considering connectivity. 
    int status = 0;
    if (ret.first){
        //if order exists, we first choose an order in which to process variables
        std::vector<std::string>  s = this->generateOrder(bindingVars);
        //Prepare iterators based on order. We must decide which trie to use and where to position iterators. 
        for (int i = 0; i < roots.size(); i++){
            
            //check whether variable or string
            std::pair<bool,bool> check = this->checkInput(bindingVars[i]);
            bool isInt1 = check.first;
            bool isInt2 = check.second;
            
            if (isInt1 && isInt2){
                std::cout << "Please do not insert relations contatining only numbers when computing joins - this is ambiguous. Two relations do not join if they do not have common variables." << std::endl;
                this->emptyJoin = true;
            
            }else if(!isInt1 && isInt2){
                if (std::find(s.begin(), s.end(), bindingVars[i][0]) == s.end()) {
                    //I don't remember what this does, probably leftover code from older versions.
                    //I think it can be removed but I'm too scared to do it before submission. 
                s.push_back(bindingVars[i][0]);
                }
                
                TrieIterator2* temp = new TrieIterator2(bindingVars[i][0],std::stoi(bindingVars[i][1]),roots2[i], status); //we choose the "flipped" trie
                if (status == 0){
                    this->varMap[bindingVars[i][0]].push_back(temp); //varMap remembers which iterators are linked to which variables. This is used for initializing levels.
                }else{
                    this->emptyJoin = true;
                    break;
                }
                
            
            }else if(isInt1 && !isInt2){
                if (std::find(s.begin(), s.end(), bindingVars[i][1]) == s.end()) {
                s.push_back(bindingVars[i][1]);
                }
                
                TrieIterator2* temp = new TrieIterator2(bindingVars[i][1],std::stoi(bindingVars[i][0]),roots[i], status);
                if (status == 0){
                    this->varMap[bindingVars[i][1]].push_back(temp);
                }else{
                    this->emptyJoin = true;
                    break;
                }
                
            
            }else{
                if (std::find(s.begin(), s.end(), bindingVars[i][0]) == s.end()) {
                s.push_back(bindingVars[i][0]);
                }
                if (std::find(s.begin(), s.end(), bindingVars[i][1]) == s.end()) {
                s.push_back(bindingVars[i][1]);
                }
                std::pair<bool,std::pair<std::string,std::string>> ordered = this->getOrdered(bindingVars[i],s); //find which one comes first in our chosen order and choose trie based on that.
                bool isReverse = ordered.first;
                if (isReverse){
                    TrieIterator2* temp = new TrieIterator2(bindingVars[i][1],bindingVars[i][0],bindingVars[i][1], roots2[i]);
                    if (bindingVars[i][0] != bindingVars[i][1]){
                        this->varMap[bindingVars[i][0]].push_back(temp);
                        this->varMap[bindingVars[i][1]].push_back(temp);
                    }else{
                        this->varMap[bindingVars[i][0]].push_back(temp);
                    }
                    
                }else{
                    TrieIterator2* temp = new TrieIterator2(bindingVars[i][0],bindingVars[i][1],bindingVars[i][0], roots[i]);
                    if (bindingVars[i][0] != bindingVars[i][1]){
                        this->varMap[bindingVars[i][0]].push_back(temp);
                        this->varMap[bindingVars[i][1]].push_back(temp);
                    }else{
                        this->varMap[bindingVars[i][0]].push_back(temp);
                    }
                }
            }
        }
        this->vars = s;
    }else{
        std::cout << "There is no join path - the answer is zero, trivially." << std::endl;
        this->emptyJoin = true;
    }
}

int TrieJoin2::joinQueries(){
    //this is where join is computed. As was described, all that we need to do is tell the right level to emit the variable

    if (!this->emptyJoin){

        QueryResult* result = new QueryResult(this->vars); //for printing

        //initialize levels
        std::vector<Level2*> levels;
        std::vector<bool>  first;
        for (int i = 0; i < this->vars.size(); i++){
            Level2* level = new Level2(this->varMap[this->vars[i]],this->vars[i]);
            levels.push_back(level);
        }
        int current = 0;
        std::vector<std::pair<std::string,int>> data;
        std::vector<int> multiplicity;
        std::pair<int,int> temp;
        
        levels[0]->initialize();
        levels[0]->initializeKey();
        int count = 0;
        //The join is computed in the below while loop
        //As we process variables in order, we emit variables and go "deeper" or "higher".
        //int current keeps track of which level emits. 
        //While loop terminates if current = 0 and we fail to emit (emit -1).
        while (true){
            if (levels[current]->num() == 1 &&!levels[current]->getEqual() && wantSort){
                //this is the mentioned optimization where we "finish off" variables of degree 1. 
                //main part of loop is the other branch.
                if (current != 0){
                    //finishing off variables of degree 1
                    int copy = current;
                    int prod = 1;
                    while (current != levels.size()){
                        prod *= levels[current]->sizeOfFamily();
                        current += 1;
                    }
                    int mult = 1;
                    for (int i =0; i < multiplicity.size(); i++){
                            mult *= multiplicity[i];
                        }

                    count += prod*mult;
                    current = copy-1;
                    multiplicity.pop_back();
                }else{
                    return levels[current]->sizeOfFamily(); //happens only in query of type R(2,x)
                }
            }
            else{
                //main part of the algorithm.
                if (current == levels.size()-1){
                    //if at last level, emit until done

                    temp = levels[current]->emit();
                    while(temp.first != -1){
                        
                        std::pair<std::string,int> temp2;
                        temp2.first = levels[current]->getVariable(); //name of variable
                        temp2.second = temp.first; //value
                        
                        if (test){
                            data.push_back(temp2); //for printing - if we actually want to visualize the data.
                        }
                        multiplicity.push_back(temp.second);
                        //we are at the end
                        int mult = 1;
                        for (int i =0; i < multiplicity.size(); i++){
                            mult *= multiplicity[i]; //multiply multiplicities across all levels. 
                        }

                        count += mult; //this is the join result
                        if (test){
                            result->insertData(data,mult);   //for printing                     
                            data.pop_back();
                        }
                        multiplicity.pop_back();
                        temp = levels[current]->emit();
                        }

                    //when finished , move to higher level 
                    if (current != 0){ 
                        current -= 1;   
                        if (test){
                            data.pop_back();
                        } 
                        multiplicity.pop_back();     
                    }else{ 
                        //only happens in case of 1 variable, in which case we are done
                        break;
                    }
                } else{
                    //ask level to emit. If value valid, go to next level. If invalid, go to previous level
                    temp = levels[current]->emit();
                    if (temp.first == -1){
                        if (current == 0){
                            break; //algorithm terminates because we are at first level and -1 was emmitted.
                        }else{
                            //we didnt find anything - remove element from previous level and go up
                            if (test){
                            data.pop_back();
                            }
                            multiplicity.pop_back();
                            current -=1; //we didnt find so we go back up
                        }
                    }else{
                        //we found something - append and go deeper
                        std::pair<std::string,int> temp2;
                        temp2.first = levels[current]->getVariable(); //name of variable
                        temp2.second = temp.first; //value
                        if (test){
                            data.push_back(temp2); //printing
                        }
                        multiplicity.push_back(temp.second); //remember multiplicity
                        
                        current += 1;
                        levels[current]->initialize(); //prepare iterators and leapfrog key on new level 
                        levels[current]->initializeKey();
                    }
                }
            }
        }
        if (test){
            result->printData();
        }
        return count;
    }else{
        return 0;
    }
}

std::pair<bool,std::pair<std::string,std::string>> TrieJoin2::getOrdered(std::vector<std::string> pair,std::vector<std::string> order){
    //checks which variable comes first under given order - this is needed to choose the "right" trie

    std::vector<std::string>::iterator it1 = std::find(order.begin(),order.end(),pair[0]);    
    std::vector<std::string>::iterator it2 = std::find(order.begin(),order.end(),pair[1]);
    std::pair<bool,std::pair<std::string,std::string>> temp;
    int index1 = it1 - order.begin();
    int index2 = it2 - order.begin();
    

    if (index1 > index2){
        temp.first = true;
        temp.second.first = order[index2];
        temp.second.second = order[index1];
        return temp;
    }else{
        temp.first = false;
        temp.second.first = order[index1];
        temp.second.second = order[index2];
        return temp;
    }
};


std::pair<bool,bool> TrieJoin2::checkInput(std::vector<std::string> pair){
    //checks whether user inputted ints or variables. This is needed so that we know how to set up our iterators.

    bool isInt1 = true;
    for (int i = 0; i < pair[0].length(); i++) {
        if(!std::isdigit(pair[0][i])){
            isInt1 = false;
        }
    }
    bool isInt2 = true;
    for (int i = 0; i < pair[1].length(); i++) {
        if(!std::isdigit(pair[1][i])){
            isInt2 = false;
        }
    }
    std::pair<bool,bool> temp;
    temp.first = isInt1;
    temp.second = isInt2;
    return temp; 
}


std::vector<std::string> TrieJoin2::generateOrder(std::vector<std::vector<std::string>> bindingVars){
    //orders the variables in descending order based on degree - this is one of the mentioned optimizations
    std::map<std::string, int> degree;
    std::vector<std::string> order;
    if (wantSort){
        
        for (int i = 0; i < bindingVars.size(); i++){
            std::pair<bool,bool> check = this->checkInput(bindingVars[i]);
            bool isInt1 = check.first;
            bool isInt2 = check.second;
            if (!isInt1){
                //this will probably crash
                degree[bindingVars[i][0]] += 1;
            }
            if (!isInt2){
                degree[bindingVars[i][1]] += 1;
            }
        }
        std::vector<std::pair<std::string, int>> v(degree.begin(), degree.end());
        sort(v.begin(), v.end(), cmp());
        return order;
    }else{
        for (int i = 0; i < bindingVars.size(); i++){
            std::pair<bool,bool> check = this->checkInput(bindingVars[i]);
            bool isInt1 = check.first;
            bool isInt2 = check.second;
            if (!isInt1){
                auto it = std::find(order.begin(), order.end(), bindingVars[i][0]);
                if (it == order.end()) {
                    order.push_back(bindingVars[i][0]);
                }
            }
            if (!isInt2){
                auto it = std::find(order.begin(), order.end(), bindingVars[i][1]);
                if (it == order.end()) {
                    order.push_back(bindingVars[i][1]);
                }
            }
        }
        
        return order;
    }
}
    



