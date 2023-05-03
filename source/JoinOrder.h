#ifndef _JOINORDER_H_
#define _JOINORDER_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>


std::pair<bool,std::vector<std::vector<std::string>>> joinOrder(std::vector<std::vector<std::string>> bindingVars, std::map<std::string, int> temp, int recursion, int maxDepth);
std::pair<bool,bool> checkInput(std::vector<std::string> pair);
std::pair<bool,std::pair<std::string,std::string>> getOrdered(std::vector<std::string> pair,std::vector<std::string> order);
std::vector<std::string> getOrder(std::vector<std::vector<std::string>> bindingVars);
std::vector<int> getPermutedIndices(std::vector<std::vector<std::string>> vars, std::vector<std::vector<std::string>> permutedVars);
std::vector<std::vector<std::vector<std::string>>> enumerateWalks(std::vector<std::vector<std::string>> bindingVars, std::map<std::string, int> temp, int recursion, int maxDepth);
#endif // _JOINORDER_H_