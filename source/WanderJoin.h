#ifndef _WANDERJOIN_H_
#define _WANDERJOIN_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Trie2.h"
#include "WanderSampler.h"


//Relation object into which we load data. Tuples are stored as keys in a map and the multiplicities are the values

class WanderJoin
{
    private:
       
        std::vector<std::vector<std::string>> bindingVars;
        bool emptyJoin = false;

        //for finding the best order
        std::vector<WanderSampler*> chosenSamplers;
        std::map<std::vector<std::string>, WanderSampler*> allSamplers;
        
        std::vector<std::vector<double>> trialEstimates;
        int numTrialRuns = 50;
        double bestDistance = 0;
        double bestFirstHit = 0;
        double bestVariance = 1000000; //just an arbitrary huge number;

        std::map<std::string, int> fixedVars;
        

    public:
        WanderJoin(std::vector<std::vector<TrieNode2*>> roots,std::vector<std::vector<TrieNode2*>> roots2, std::vector<std::vector<std::string>> bindingVars);
        ~WanderJoin(); 
        double estimateJoin(int number);
        std::vector<std::vector<double>> estimateJoin(int number, bool test);
        std::pair<double,std::pair<int,int>> estimateJoin();
        void fixVars(std::vector<std::string> vars, std::pair<int,int> tuple);
        void initializeJoin(std::vector<std::vector<TrieNode2*>> roots,std::vector<std::vector<TrieNode2*>> roots2, std::vector<std::vector<std::string>> bindingVars);
        std::pair<bool,std::vector<std::vector<std::string>>> pickOrder(std::vector<std::vector<std::string>> bindingVars, std::string mode);
        bool isBestDistance(std::vector<double> distances);
        bool isbestFirstHit(std::vector<double> firsthits);
        bool isbestVariance(std::vector<double> estimates);
};
#endif // _WANDERJOIN_H_