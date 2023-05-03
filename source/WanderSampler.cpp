#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Trie2.h"
#include "WanderSampler.h"
#include <random>

WanderSampler::WanderSampler(std::string var1, std::string var2, std::vector<TrieNode2*> root){
    //we assume var1 is the "top" one in the root - this is like in the TrieIterators
    this->var1 = var1;
    this->var2 = var2;
    this->root = root;
    this->equal = var1 == var2;
}

WanderSampler::WanderSampler(std::string var1, int val, std::vector<TrieNode2*> root){
    //we assume var1 is the "top" one in the root
    this->var1 = var1;
    this->hasVal = val;
    this->root = root;
}

double WanderSampler::sample(std::map<std::string,int>* fixedVars){
    //c1 c2 represent whether the vars of this relation have been "fixed" or not
    if (this->hasVal == -1){

        int c1 = fixedVars->count(this->var1);
        int c2 = fixedVars->count(this->var2);
        if (c1 > 0){
            int val1 = fixedVars->at(var1); //the value to which it has been fixed before;
            //we do binary search 

            auto search1 = std::lower_bound(this->root.begin(),this->root.end(), val1, less_than());
            const bool found1 = search1 != this->root.end() && (*search1)->value == val1;
            if (found1){
                if (c2 > 0){
                    int val2 = fixedVars->at(this->var2);
                    auto search2 = std::lower_bound((*search1)->family.begin(),(*search1)->family.end(), val2, less_than());
                    const bool found2 = search2 != (*search1)->family.end() && (*search2)->value == val2;
                    if (found2){
                        return (*search2)->multiplicity; //if we find both, we must return multiplicity
                    }else{
                        // the previously fixed value isn't there, so path is unsuccesful
                        return 0;
                    }
                }else{
                    //if one value has been fixed but the other hasn't we sample it (and treat multiplicity)
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dis(0, (*search1)->family.size() - 1);
                    int random_index = dis(gen);
                    int random_element = (*search1)->family[random_index]->value;
                    fixedVars->insert({this->var2,random_element});
                    return (*search1)->family.size()*(*search1)->family[random_index]->multiplicity;
                }
            }else{
                return 0;  // the previously fixed value isn't there, so path is unsuccesful
            }
        }else{
            //if the value hasn't been fixed, we sample it. We know the other is also not fixed because of the upper var always comes first in the order.
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, this->root.size() - 1);
            int random_index1 = dis(gen);
            int random_element1 = this->root[random_index1]->value;
            double prob1 = 1.0*this->root.size();

            fixedVars->insert({this->var1,random_element1});
            
            std::uniform_int_distribution<> dis2(0, this->root[random_index1]->family.size() - 1);
            int random_index2 = dis2(gen);
            int random_element2 = this->root[random_index1]->family[random_index2]->value;
            double prob2 = 1.0*this->root[random_index1]->family.size()*this->root[random_index1]->family[random_index2]->multiplicity; //explained in Q1 c)
            fixedVars->insert({this->var2,random_element2});
            
            if (this->equal){
                if (random_element1 != random_element2){
                    return 0;
                }
            }
            return prob1*prob2;
        }
    }else{
        //corresponds to case R(2,x) - we must search for the fixed value.
        return this->sample2(fixedVars);
    }
}
double WanderSampler::sample2(std::map<std::string,int>* fixedVars){
    //corresponds to the case when one variable is int and the other is a string

    auto search = std::lower_bound(this->root.begin(),this->root.end(), this->hasVal, less_than());
    const bool found = search != this->root.end() && (*search)->value == this->hasVal;
    if (found){
        int c1 = fixedVars->count(this->var1);
        if (c1 > 0){
            //check if fixed value is in corresponding branch (the family of the upper)
            int val = fixedVars->at(this->var1);
            auto search2 = std::lower_bound((*search)->family.begin(),(*search)->family.end(), val, less_than());
            const bool found2 = search2 != (*search)->family.end() && (*search2)->value == val;
            if (found2){
                return (*search2)->multiplicity; //we return multiplicity if fixed
            }else{
                // the previously fixed value isn't there, so path unsuccesful;
                return 0; 
            }
        }else{
            //if it hasn't been fixed, we sample it.
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, (*search)->family.size() - 1);
            int random_index = dis(gen);
            int random_element = (*search)->family[random_index]->value;
            
            fixedVars->insert({this->var1,random_element});
            return  (*search)->family.size()*(*search)->family[random_index]->multiplicity;
        }
    }else{
        return 0; //no such fixed value exists, so it is 0. 
    }
}