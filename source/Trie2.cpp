#include "Trie2.h" 
#include <iostream>

#include <map>
#include <vector>
#include <string>





Trie2::Trie2(std::string Name){
    this->Name = Name;
}


void Trie2::insertTuple(int parentValue, int childValue){
    this->numTuples += 1;
    auto low = std::lower_bound(this->root.begin(), this->root.end(), parentValue, less_than());
    if (low == this->root.end()){
        //we didnt find it;
        TrieNode2* child = new TrieNode2();
        child->value = childValue;
        child->multiplicity = 1;
        
        TrieNode2* parent = new TrieNode2();
        parent->value = parentValue;
        parent->multiplicity = 1; 
        parent->sizeOfFamily = 1; 
        parent->family.push_back(child);

        //insert into sorted
        //auto up = std::upper_bound(this->root.begin(), this->root.end(), parentValue, greater_than()); //low+1?
        this->root.push_back(parent);
        child->family.push_back(parent);
    
    }else if((*low)->value != parentValue){
        //we didnt find it;
        TrieNode2* child = new TrieNode2();
        child->value = childValue;
        child->multiplicity = 1;
        
        TrieNode2* parent = new TrieNode2();
        parent->value = parentValue;
        parent->multiplicity = 1; 
        parent->sizeOfFamily = 1; 
        parent->family.push_back(child);

        //insert into sorted
        //auto up = std::upper_bound(this->root.begin(), this->root.end(), parentValue, greater_than()); //low+1?
        this->root.insert(low, parent);
        child->family.push_back(parent);
    }
    
    else if((*low)->value == parentValue){
        auto low2 = std::lower_bound((*low)->family.begin(), (*low)->family.end(), childValue, less_than());
        if (low2 == (*low)->family.end()){
            TrieNode2* child = new TrieNode2();
            child->value = childValue;
            child->multiplicity = 1;
            child->family.push_back(*low);
            
           // auto up = std::upper_bound((*low)->family.begin(), (*low)->family.end(), childValue, greater_than());//low+1?
            (*low)->family.push_back(child);
            (*low)->sizeOfFamily += 1;
            
        
        
        }else if ((*low2)->value != childValue){
            TrieNode2* child = new TrieNode2();
            child->value = childValue;
            child->multiplicity = 1;
            child->family.push_back(*low);
            
           // auto up = std::upper_bound((*low)->family.begin(), (*low)->family.end(), childValue, greater_than());//low+1?
            (*low)->family.insert(low2, child);
            (*low)->sizeOfFamily += 1;
        }else{
            (*low2)->multiplicity +=1;
            (*low)->sizeOfFamily += 1;
        }
    }
    this->insertTuple2(childValue,parentValue);
}


void Trie2::insertTuple2(int parentValue, int childValue){
    auto low = std::lower_bound(this->root2.begin(), this->root2.end(), parentValue, less_than());
    if (low == this->root2.end()){
        //we didnt find it;
        TrieNode2* child = new TrieNode2();
        child->value = childValue;
        child->multiplicity = 1;
        
        
        TrieNode2* parent = new TrieNode2();
        parent->value = parentValue;
        parent->multiplicity = 1;
        parent->sizeOfFamily = 1; 
        parent->family.push_back(child);

        //insert into sorted
        //auto up = std::upper_bound(this->root.begin(), this->root.end(), parentValue, greater_than()); //low+1?
        this->root2.push_back(parent);
        child->family.push_back(parent);
    
    }else if((*low)->value != parentValue){
        //we didnt find it;
        TrieNode2* child = new TrieNode2();
        child->value = childValue;
        child->multiplicity = 1;
        
        TrieNode2* parent = new TrieNode2();
        parent->value = parentValue;
        parent->multiplicity = 1; 
        parent->sizeOfFamily = 1;
        parent->family.push_back(child);

        //insert into sorted
        //auto up = std::upper_bound(this->root.begin(), this->root.end(), parentValue, greater_than()); //low+1?
        this->root2.insert(low, parent);
        child->family.push_back(parent);
    }
    
    else if((*low)->value == parentValue){
        auto low2 = std::lower_bound((*low)->family.begin(), (*low)->family.end(), childValue, less_than());
        if (low2 == (*low)->family.end()){
            TrieNode2* child = new TrieNode2();
            child->value = childValue;
            child->multiplicity = 1;
            child->family.push_back(*low);
            
           // auto up = std::upper_bound((*low)->family.begin(), (*low)->family.end(), childValue, greater_than());//low+1?
            (*low)->family.push_back(child);
            (*low)->sizeOfFamily += 1;
        
        }else if ((*low2)->value != childValue){
            TrieNode2* child = new TrieNode2();
            child->value = childValue;
            child->multiplicity = 1;
            child->family.push_back(*low);
            
           // auto up = std::upper_bound((*low)->family.begin(), (*low)->family.end(), childValue, greater_than());//low+1?
            (*low)->family.insert(low2, child);
            (*low)->sizeOfFamily += 1;
        }else{
            (*low2)->multiplicity +=1;
            (*low)->sizeOfFamily += 1;
        }
    }
}


int Trie2::retrieveTuple(int parentValue, int childValue){
    auto low = std::lower_bound(this->root.begin(), this->root.end(), parentValue, less_than());
    if (low == this->root.end() || (*low)->value != parentValue){
        return 0;
    }else{
        auto low2 = std::lower_bound((*low)->family.begin(), (*low)->family.end(), childValue, less_than());
        if (low2 == (*low)->family.end() || (*low2)->value != childValue){
            return 0;
        }else{
            return (*low2)->multiplicity;
        }
    }
}

void Trie2::printData(){
    std::cout << "printing" << std::endl;
    for (int i  = 0; i < this->root.size();i++){
        for (int j = 0; j < this->root[i]->family.size(); j++){
            std::cout << this->root[i]->value<< " " << this->root[i]->family[j]->value << "With multiplicity" << this->root[i]->family[j]->multiplicity << std::endl;
        }
    }
}

void Trie2::printData2(){
    std::cout << "printing" << std::endl;
    for (int i  = 0; i < this->root2.size();i++){
        for (int j = 0; j < this->root2[i]->family.size(); j++){
            std::cout << this->root2[i]->value<< " " << this->root2[i]->family[j]->value << "With multiplicity" << this->root2[i]->family[j]->multiplicity << std::endl;
        }
    }
}


 std::vector<TrieNode2*> Trie2::getRoot(){
    return this->root;
 }

 std::vector<TrieNode2*> Trie2::getRoot2(){
    return this->root2;
 }

 int Trie2::getNum(){
    return this->numTuples;
 }