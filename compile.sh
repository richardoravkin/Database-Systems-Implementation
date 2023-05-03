#!/bin/bash

# Compile the C++ program
cd ./source/
g++ --std=c++17  main.cpp queryparser.cpp queryresult.cpp triejoin2.cpp level2.cpp trieiterator2.cpp trie2.cpp wandersampler.cpp wanderjoin.cpp joinorder.cpp -o a.out

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Run file ./a.out"
else
    echo "Compilation failed"
fi