# include "JoinOrder.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>

//Some auxillary functions - the important ones are enumerateWalks and joinOrder.
//they are very similar. It is just a simple recursion. I know it looks ugly but it is quite simple. 
//basically you just recursively call the function but pass it the order which has been generated so far
//it then tries to find an order of the remaining relations so that it is ok with respect to the order generated so far


std::pair<bool,std::vector<std::vector<std::string>>> joinOrder(std::vector<std::vector<std::string>> bindingVars, std::map<std::string, int> temp, int recursion, int maxDepth){
   
    
    for (int i = 0; i < bindingVars.size(); i++){
        std::pair<bool,bool> check = checkInput(bindingVars[i]);
        bool isInt1 = check.first;
        bool isInt2 = check.second;
        if (recursion == 0){
            if (recursion == maxDepth){
                std::pair<bool,std::vector<std::vector<std::string>>> temp2;
                temp2.first = true;
                temp2.second = bindingVars;
                return temp2;
            }else{
                std::map<std::string, int> temp3 = temp;
                if (!isInt1){
                    temp3.insert({bindingVars[i][0],1});
                }
                if (!isInt2){
                    temp3.insert({bindingVars[i][1],1});
                }
                std::vector<std::vector<std::string>> slice = bindingVars;
                slice.erase(slice.begin() + i);
                std::pair<bool,std::vector<std::vector<std::string>>> temp2 = joinOrder(slice,temp3, recursion + 1, maxDepth);
                if (temp2.first){
                    temp2.second.insert(temp2.second.begin(), bindingVars[i]);
                    return temp2;
                }
            }
        }else{
            
            std::pair<bool,bool> check = checkInput(bindingVars[i]);
            bool isInt1 = check.first;
            bool isInt2 = check.second;
            bool flag = false;
            int c1 = -1;
            int c2 = -1;
            if (!isInt1){
                c1 = temp.count(bindingVars[i][0]); //check whether variable occured in previous
                if (c1 > 0){
                    flag = true;
                }
            }
            if (!isInt2){
                c2 = temp.count(bindingVars[i][1]); //check whether variable occured in previous
                if (c2 > 0){
                    flag = true;
                }
            }
            
            if (flag){ //if neither, then go to next element in loop
                std::map<std::string, int> temp3 = temp;
                if (c1 == 0){
                    temp3.insert({bindingVars[i][0],1}); //insert the ones which were not there
                }

                if (c2 == 0){
                    temp3.insert({bindingVars[i][1],1});
                }
                
                if (recursion != maxDepth){
                    std::vector<std::vector<std::string>> slice = bindingVars;
                    slice.erase(slice.begin() + i);
                    std::pair<bool,std::vector<std::vector<std::string>>> temp2 = joinOrder(slice,temp3, recursion + 1, maxDepth);
                    if (temp2.first){
                        temp2.second.insert(temp2.second.begin(), bindingVars[i]);
                        return temp2;
                    }   
                }else{
                    std::pair<bool,std::vector<std::vector<std::string>>> temp2;
                    temp2.first = true;
                    temp2.second = bindingVars;
                    return temp2;
                }
            }
        }
    }
    //if we ever finish the loop, we did not find a join order and hence must propagate back
    std::pair<bool,std::vector<std::vector<std::string>>> temp2;
    temp2.first = false;
    temp2.second = bindingVars;
    return temp2;
}


std::pair<bool,bool> checkInput(std::vector<std::string> pair){
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

std::pair<bool,std::pair<std::string,std::string>> getOrdered(std::vector<std::string> pair,std::vector<std::string> order){
    
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


std::vector<std::string> getOrder(std::vector<std::vector<std::string>> bindingVars){
    std::vector<std::string> output;
    
    for (auto pair:bindingVars){

        std::pair<bool,bool> check = checkInput(pair);
        bool isInt1 = check.first;
        bool isInt2 = check.second;

        if (!isInt1 && std::count(output.begin(),output.end(),pair[0]) == 0){
            output.push_back(pair[0]);
        }
        if (!isInt2 && std::count(output.begin(),output.end(),pair[1]) == 0){
            output.push_back(pair[1]);
        }
    }
    return output;
}



std::vector<int> getPermutedIndices(std::vector<std::vector<std::string>> vars, std::vector<std::vector<std::string>> permutedVars){
    std::vector<int> flag(vars.size(),1);
    std::vector<int> output(vars.size(),-1);
    for(int i = 0; i < vars.size(); i++){
        for (int j = 0; j < permutedVars.size(); j++){
            if (vars[i] == permutedVars[j] && flag[j] == 1){
                output[i] = j;
                flag[j] = 0;
                break;
            }
        }
    }
    return output; 
 }


std::vector<std::vector<std::vector<std::string>>> enumerateWalks(std::vector<std::vector<std::string>> bindingVars, std::map<std::string, int> temp, int recursion, int maxDepth){
   //very similar to join order, but you return ALL walks instead of just one. 
    std::vector<std::vector<std::vector<std::string>>> res;
    for (int i = 0; i < bindingVars.size(); i++){
        std::pair<bool,bool> check = checkInput(bindingVars[i]);
        bool isInt1 = check.first;
        bool isInt2 = check.second;
        if (recursion == 0){
            if (recursion == maxDepth){
                std::vector<std::vector<std::string>> temp1;
                temp1.push_back(bindingVars[i]);
                res.push_back(temp1);
            }else{
                std::map<std::string, int> temp3 = temp;
                if (!isInt1){
                    temp3.insert({bindingVars[i][0],1});
                }
                if (!isInt2){
                    temp3.insert({bindingVars[i][1],1});
                }
                std::vector<std::vector<std::string>> slice = bindingVars;
                slice.erase(slice.begin() + i);
                std::vector<std::vector<std::vector<std::string>>> temp2 = enumerateWalks(slice,temp3, recursion + 1, maxDepth);
                for (auto vec:temp2){
                    vec.insert(vec.begin(),bindingVars[i]);
                    res.push_back(vec);
                }
                
            }
        }else{
            
            std::pair<bool,bool> check = checkInput(bindingVars[i]);
            bool isInt1 = check.first;
            bool isInt2 = check.second;
            bool flag = false;
            int c1 = -1;
            int c2 = -1;
            if (!isInt1){
                c1 = temp.count(bindingVars[i][0]); //check whether variable occured in previous
                if (c1 > 0){
                    flag = true;
                }
            }
            if (!isInt2){
                c2 = temp.count(bindingVars[i][1]); //check whether variable occured in previous
                if (c2 > 0){
                    flag = true;
                }
            }
            
            if (flag){ //if neither, then go to next element in loop
                std::map<std::string, int> temp3 = temp;
                if (c1 == 0){
                    temp3.insert({bindingVars[i][0],1}); //insert the ones which were not there
                }

                if (c2 == 0){
                    temp3.insert({bindingVars[i][1],1});
                }
                
                if (recursion != maxDepth){
                    std::vector<std::vector<std::string>> slice = bindingVars;
                    slice.erase(slice.begin() + i);
                    std::vector<std::vector<std::vector<std::string>>> temp2 = enumerateWalks(slice,temp3, recursion + 1, maxDepth);
                    
                    for (auto vec:temp2){
                        vec.insert(vec.begin(),bindingVars[i]);
                        res.push_back(vec);
                    }     
                }else{
                    std::vector<std::vector<std::string>> temp1;
                    temp1.push_back(bindingVars[i]);
                    res.push_back(temp1);
                }
            }
            
        }
    }
    return res;
}