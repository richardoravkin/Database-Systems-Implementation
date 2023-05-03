#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <filesystem>
//#include "Relation.h" 
#include "QueryParser.h" 
#include <cctype>
//#include "Join.h" 
#include "Trie2.h" 
#include "TrieJoin2.h" 
#include "WanderJoin.h" 
#include <chrono>
#include "JoinOrder.h"


namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono;
int main(){
    
    //typedef std::map< std::string, Relation*> RelationMap;
    typedef std::map< std::string, Trie2*> RelationMap;
    RelationMap Relations; 
    std::map<std::string,int> numsTuples;
    while (true){
       
        QueryParser* parser = new QueryParser();
        string command;
        command = parser->getQuery()[0];
        
        if (command == "QUIT"){
            break;
        } else if (command == "LOAD"){
            
            if (parser->getQuery().size() < 3){
                cerr << "Invalid syntax: Please use the LOAD rel_name rel_file format when loading" << endl;
            }else{
                string relname = parser->getQuery()[1];
                string filename = parser->getQuery()[2];
                
                ifstream myfile;
                string myline;
                string dir("../data/"); //possibly change this if you are loading data from elsewhere. Also, I don't know what will happen if you load data from different filetypes. I used it only on the provided data. 
                fs::path fullPath = dir;
                fullPath /= filename;
            
                myfile.open(fullPath.c_str());
                if (!myfile){
                    std::cerr << "Problem opening file: Check your spelling!" << endl;
                } else{
                    int numTuples = 0;
                    if (Relations.find(relname) == Relations.end()){
                        //Relation* R1 = new Relation(relname);
                        //Relations[relname] = R1;
                        Trie2* R1 = new Trie2(relname);
                        Relations[relname] = R1;
                    }
                    
                    if ( myfile.is_open() ) 
                    {   
                        auto start_time = high_resolution_clock::now();
                        while ( getline (myfile, myline) ) 
                        { 
                            std::istringstream ss(myline);
                            std::string token;
                            std::vector<int> x;   
                            while(std::getline(ss, token, ',')) {
                                x.push_back(stoi(token));
                            }
                            numTuples += 1;
                            
                            Relations[relname]->insertTuple(x[0],x[1]);
                            
                        }
                        //Relations[relname]->printData();
                        auto end_time = high_resolution_clock::now();
                        auto elapsed_time = duration_cast<milliseconds>(end_time - start_time);
                        cout << "Successfully inserted " << numTuples << " tuples " << "in " << elapsed_time.count() << " miliseconds." << endl; //should get sum num of tuples
                        
                    }   
                }
            }
        }else if (command == "COUNT"){
            // parsing relations
            std::vector<pair<string,vector<string>>> temp;
            std::vector<std::vector<std::string>> bindingVars;
            
            //std::vector<std::vector<TrieNode2*>> roots;
            std::vector<std::vector<TrieNode2*>> roots;
            std::vector<std::vector<TrieNode2*>> roots2;
            bool flag = true;
            for (int i = 1; i < parser->getQuery().size(); i ++){
                temp.push_back(parser->parseRelation(i));
                if (Relations.find(temp[i-1].first) == Relations.end()){
                    cout << "One of the requested relations has not beed loaded yet. Please check you spelling or load the relation first." << endl;
                    flag = false;
                    break;
                } else{
                    bindingVars.push_back(temp[i-1].second);
                    roots.push_back(Relations[temp[i-1].first]->getRoot());
                    roots2.push_back(Relations[temp[i-1].first]->getRoot2());
                    
                    //processed.push_back(Relations[temp.first]->processQuery(temp.second)); //->printData()
                }
            }
            if (roots.size() == 1){
                std::pair<bool,bool> check = checkInput(bindingVars[0]);
                bool isInt1 = check.first;
                bool isInt2 = check.second;
                if (!isInt1 && !isInt2 && bindingVars[0][0] != bindingVars[0][1]){
                    cout<< "Number of tuples is just the size of the relation " << Relations[temp[0].first]->getNum() << endl;
                    flag = false;
                }
                if (isInt1 && isInt2){
                    cout << "The multiplicity of this tuple is " << Relations[temp[0].first]->retrieveTuple(stoi(bindingVars[0][0]),stoi(bindingVars[0][1])) << endl;
                    flag = false;
                }
                
            }
        

            if (flag){
                auto start_time = high_resolution_clock::now();
                TrieJoin2* join = new TrieJoin2(roots,roots2,bindingVars);
                
                int num = join->joinQueries(); 
                auto end_time = high_resolution_clock::now();
                auto elapsed_time = duration_cast<milliseconds>(end_time - start_time);
                cout<< "Number of tuples is " << num;
                cout << " computed in " << elapsed_time.count() << " miliseconds." << std::endl; //->printData();
                
            }
        }else if (command == "ESTIMATE"){
            pair<string,vector<string>> temp;
            std::vector<std::vector<std::string>> bindingVars;
            bool flag = true;
            std::vector<std::vector<TrieNode2*>> roots;
            std::vector<std::vector<TrieNode2*>> roots2;
            for (int i = 1; i < parser->getQuery().size(); i ++){
                temp = parser->parseRelation(i);
                if (Relations.find(temp.first) == Relations.end()){
                    cout << "One of the requested relations has not beed loaded yet. Please check you spelling or load the relation first." << endl;
                    flag = false;
                    break;
                } else{
                    bindingVars.push_back(temp.second);
                    roots.push_back(Relations[temp.first]->getRoot());
                    roots2.push_back(Relations[temp.first]->getRoot2());
                }
            }

            if (flag){
                auto start_time = high_resolution_clock::now();
                WanderJoin* join = new WanderJoin(roots,roots2,bindingVars);
                auto end_time = high_resolution_clock::now();
                auto elapsed_time = duration_cast<milliseconds>(end_time - start_time);
                cout << " Elapsed time " << elapsed_time.count() << " miliseconds." << endl;
                
                
            }
        }else{
            cout << "Invalid command, please check your spelling. The commands are case sensitive." << endl;
        }
    }   
    return 0;
}
 