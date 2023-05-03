#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Trie2.h"
#include "WanderJoin.h"
#include "WanderSampler.h"
#include "JoinOrder.h"
#include <chrono>

//IMPORTANT: In the below function, the commented out sections correspond to the different selection criterions. 
bool optimization = true; //change this to false if you want vanilla version of Wander Join

WanderJoin::WanderJoin(std::vector<std::vector<TrieNode2*>> roots,std::vector<std::vector<TrieNode2*>> roots2,std::vector<std::vector<std::string>> bindingVars){
    
    this->initializeJoin(roots,roots2,bindingVars);
    
    // std::pair<bool,std::vector<std::vector<std::string>>> best_order = this->pickOrder(bindingVars, "VARIANCE");
    // if (best_order.first){
    //     std::cout << "The walk order with lowest variance from trials is: ";
    //     for (auto o:best_order.second){
    //         std::cout << o[0] << o[1] << " ";
    //     }
    //     std::cout << "." << std::endl;
    //     //this->initializeJoin(roots,roots2,bindingVars,best_order.second);
    //     std::cout << "With this order, estimated number is: " << this->estimateJoin(1000) << std::endl;
        
    // }else{
    //     this->emptyJoin = true;
    //     this->estimateJoin(1); //to return 0;
    // }
    // //reset

    std::vector<std::vector<double>> vec;
    this->trialEstimates = vec;
    std::pair<bool,std::vector<std::vector<std::string>>> best_order = this->pickOrder(bindingVars, "DISTANCE"); //picks best order
    if (best_order.first){
        std::cout << "The estimated number is: " << this->estimateJoin(1000) << std::endl; //computes 1000 runs 
        
    }else{
        this->emptyJoin = true;
        this->estimateJoin(1); //to return 0;
    }

    //reset

    // this->trialEstimates = vec;
    // best_order = this->pickOrder(bindingVars, "FIRST HITS");
    // if (best_order.first){
    //     std::cout << "The order with most first hits from trials is: ";
    //     for (auto o:best_order.second){
    //         std::cout << o[0] << o[1] << " ";
    //     }
    //     std::cout << "." << std::endl;
    //     //this->initializeJoin(roots,roots2,bindingVars,best_order.second);
    //     std::cout << "With this order, estimated number is: " << this->estimateJoin(1000) << std::endl;
        
    // }else{
    //     this->emptyJoin = true;
    //     this->estimateJoin(1); //to return 0;
    // }
}

void WanderJoin::initializeJoin(std::vector<std::vector<TrieNode2*>> roots, std::vector<std::vector<TrieNode2*>> roots2,std::vector<std::vector<std::string>> bindingVars){
     //this is very similar to TrieJoin2, only now we don't initialize iterators but Wander Sampler instances. 

    for (int i = 0; i < roots.size(); i++){
        
        //check whether variable or string
        std::pair<bool,bool> check = checkInput(bindingVars[i]);
        bool isInt1 = check.first;
        bool isInt2 = check.second;
        
        if (isInt1 && isInt2){
            std::cout << "Please do not insert relations contatining only numbers when computing joins - this is ambiguous. Two relations do not join if they do not have common variables. Returning 0." << std::endl;
            this->emptyJoin = true;

        }else if(!isInt1 && isInt2){
            WanderSampler* temp = new WanderSampler(bindingVars[i][0],std::stoi(bindingVars[i][1]),roots2[i]);
            
            this->allSamplers[bindingVars[i]] = temp; //all samplers is used for the different join orders. You possibly need all combinations. 
            
        }else if(isInt1 && !isInt2){
            WanderSampler* temp = new WanderSampler(bindingVars[i][1],std::stoi(bindingVars[i][0]),roots[i]);
            std::vector<std::string> reverse;
            reverse.push_back(bindingVars[i][1]);
            reverse.push_back(bindingVars[i][0]);
            this->allSamplers[reverse] = temp; 
        
        }else{
            WanderSampler* temp1 = new WanderSampler(bindingVars[i][1],bindingVars[i][0], roots2[i]);
            std::vector<std::string> reverse;
            reverse.push_back(bindingVars[i][1]);
            reverse.push_back(bindingVars[i][0]);
            this->allSamplers[reverse] = temp1;
        
            WanderSampler* temp2 = new WanderSampler(bindingVars[i][0],bindingVars[i][1], roots[i]);
            this->allSamplers[bindingVars[i]] = temp2;
            
        }
   
    }
}

std::vector<std::vector<double>> WanderJoin::estimateJoin(int number, bool test){
    //for collecting statistics during trial runs

    std::vector<double> estimates;
    std::vector<double> distances;
    std::vector<double> firstHits;
    double avg_first = 0;
    for (int i = 0; i < number; i++){
        std::pair<double,std::pair<int,int>> res = this->estimateJoin();
        estimates.push_back(res.first);
        distances.push_back(res.second.first);
        firstHits.push_back(res.second.second);
        
    }
    std::vector<std::vector<double>> res;
    res.push_back(estimates);
    res.push_back(distances);
    res.push_back(firstHits);
    return res;
  
}


double WanderJoin::estimateJoin(int number){
    if (!this->emptyJoin){
        double avg = 0;
        for (int i = 0; i < number; i++){
            avg += this->estimateJoin().first;
        }
        for (auto estimates:this->trialEstimates){
            for (auto e:estimates){
                avg += e; //we don't waste the trials
            }
        }

        return avg/(number+this->trialEstimates.size()*this->numTrialRuns);
    }else{
        return 0;
    }
}
std::pair<double,std::pair<int,int>> WanderJoin::estimateJoin(){
    //this is where we actually compute the estimates. We call the right wander sampler instance to sample 
    //and give us a probability and a multiplicity and we multiply these to get estimate 

    double prob = 1.0;
    int count = 0;
    int firstHit = 0; 
    std::map<std::string,int> fixedVars; //these represent the variables which are fixed by previous samples. Explained in report. 
    
    while (prob != 0 && count < this->chosenSamplers.size()){
        double val = this->chosenSamplers[count]->sample(&fixedVars); //we ask it to sample
        prob *= val;
        count += 1;
        if (val != 0 && count == 2){
            firstHit = 1; //for statistics about first hits
        }
    }
    
    std::pair<double,std::pair<int,int>> res;
    res.first = prob;
    std::pair<int,int> temp;
    temp.first = count;
    temp.second = firstHit;
    res.second = temp;
    return res;  
}


std::pair<bool,std::vector<std::vector<std::string>>> WanderJoin::pickOrder(std::vector<std::vector<std::string>> bindingVars, std::string mode){
    //for picking the best order
    std::map<std::string, int> temp;
    std::vector<std::vector<std::vector<std::string>>> walks;
    if (optimization){
         walks = enumerateWalks(bindingVars,temp, 0, bindingVars.size()-1); //we enumerate all walks. Look at JoinOrder.cpp
    }else{
        //if we dont optimize, we simply check if a walk order exists but dont enumerate all
        std::pair<bool,std::vector<std::vector<std::string>>> ret = joinOrder(bindingVars,temp, 0, bindingVars.size()-1);
        if (ret.first){
            walks.push_back(joinOrder(bindingVars,temp, 0, bindingVars.size()-1).second);
        }   
    }
    
    if (walks.size() == 0){
        std::pair<bool,std::vector<std::vector<std::string>>> res;
        res.first = false;
        return res;
    }else{
        std::vector<std::vector<std::string>> best_walk = walks[0];
        std::vector<WanderSampler*> bestVec;

        for (std::vector<std::vector<std::string>> walk:walks){        
            
            //prepare wander join for each walk;

            std::vector<WanderSampler*> vec;
            this->chosenSamplers = vec;
            std::vector<std::string> s = getOrder(walk); 
            for (std::vector<std::string> vars:walk){    
                std::pair<bool,std::pair<std::string,std::string>> ordered = getOrdered(vars,s); //find which one comes first
                bool isReverse = ordered.first;
                if (isReverse){
                    std::vector<std::string> temp;
                    temp.push_back(vars[1]);
                    temp.push_back(vars[0]);
                    this->chosenSamplers.push_back(this->allSamplers[temp]); //we choose the right samplers from all samplers
                }else{
                    this->chosenSamplers.push_back(this->allSamplers[vars]);
                }
            }

            //compute numTrialRuns for given walk and record statistics.
            std::vector<std::vector<double>> res = this->estimateJoin(this->numTrialRuns, true);
            std::vector<double> estimates = res[0];
            std::vector<double> distances = res[1];
            std::vector<double> firstHits = res[2];
            
            //we don't want to waste trials
            this->trialEstimates.push_back(estimates); //

            //look for best walk based on desired mode
            if (mode == "VARIANCE"){
                if (this->isbestVariance(estimates)){
                    best_walk = walk;
                    bestVec = this->chosenSamplers;
                }
            }else if (mode == "DISTANCE"){
                if (this->isBestDistance(distances)){
                    best_walk = walk;
                    bestVec = this->chosenSamplers;
                }
            }else if (mode == "FIRST HIT"){
                if (this->isbestFirstHit(firstHits)){
                    best_walk = walk;
                    bestVec = this->chosenSamplers;
                }
            }
        }
        //return best walk order
        std::pair<bool,std::vector<std::vector<std::string>>> res;
        res.first = true;
        res.second = best_walk;
        this->chosenSamplers = bestVec;
        return res;
    }
}


bool WanderJoin::isBestDistance(std::vector<double> distances){
    //best walk is the one with the largest average distance
    double avg_distance = 0;
    for (double d:distances){
        avg_distance += d;
    }
    avg_distance /= distances.size();
    if (avg_distance > this->bestDistance){
        this->bestDistance = avg_distance;
        return true;
    }
    return false;
    
}
bool WanderJoin::isbestFirstHit(std::vector<double> firsthits){
    //best walk is the one with the largest average number of first hits
    double avg_firsthits = 0;
    for (double d:firsthits){
        avg_firsthits += d;
    }
    avg_firsthits /= firsthits.size();
    if (avg_firsthits > this->bestFirstHit){
        this->bestFirstHit = avg_firsthits;
        return true;
    }
    return false;

}
bool WanderJoin::isbestVariance(std::vector<double> estimates){
    //best walk is the one with the lowest variance
    double variance = 0;
    double sample_mean = 0;
    for (double e:estimates){
        sample_mean += e;
    }
    sample_mean /= estimates.size();

    for (double e:estimates){
        variance += (e-sample_mean)*(e-sample_mean);
    }
    if (estimates.size() > 1){
        variance /= estimates.size()-1; //unbiased estimator of variance is 1/(n-1)*sum... not 1/n*sum. Also in Wander Join paper.
    }else{
        std::cout << "Warning - a sample of 1 experiment does not have a variance. Increase number of experiments for more accurate predictions." << std::endl;
    }

    if (variance < this->bestVariance){
        this->bestVariance = variance;
        return true;
    }
    return false;
}