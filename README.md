# Database-Systems-Implementation
This repository contains my solution to the Database Systems Implementation coursework, offered at the University Of Oxford in Hilary 2023. 

The purpose of the exam was to build an in-memory database which allows for efficient computation of joins. The exam had two main tasks: 
The first was to implement a join algorithm which can efficiently evaluate reasonably complicated joins for a moderately sized dataset. 
The second task was to implement the "Wanderjoin Algorithm" (https://dl.acm.org/doi/10.1145/2882903.2915235), which can estimate the size of the join much faster than the exact algorithm. 

As a solution to the first task, I implemented a variation of the "Leapfrog TrieJoin" (https://arxiv.org/abs/1210.0481), which is a join algorithm known to be "worst case optimal". 
My solution to the second task closely tracks the approach presented in the Wanderjoin paper. The suggested optimizations were also implemented. 

All code in this repository was written from scratch, by me. The examiners only provided us with the dataset and some queries for testing. 


# How to use this repo
If you would like to try it out, go to /source and run ./a.out in Terminal. If you modify the code for any reason, you will need to recompile by running the compile.bash file. 
Once you run the ./a.out, you can start loading the data. This is done by running LOAD <rel_name> <file_name>, where <file_name> is the name of the file containing the data and is located in the /data folder. 
The database only supports data in the form of tuples of integers. The file 'test' is provided as an example. 

Once the data is loaded, you can start running queries. Two types of queries are supported. The first type is the exact computation of joins, which is called by the 'COUNT' keyword.
The second is the estimation of the size of a join, which is called by the keyword 'ESTIMATE'. 


As an example, suppose we want to find all 'triangles' in the file 'test'. We would do this as follows:

./a.out

LOAD test test

COUNT test(x,y) test(y,z) test(z,x) 

Now suppose, we want an approximate answer to the number of tuples where the first integer is 2, and the second is something which also occurs on the first place in a tuple which has a 3 on the second place.
We now run:

ESTIMATE test(2,x) test(x,3) 

Note that the algorithms correctly handle tuples which are repeated - this is accounted for in the multiplicity. 

In case you want to run this on proper data, make sure to disable printing by modifying the code. Otherwise the performance will be slower. 

The code was tested on a dataset where relations were of size 10-60MB. Files which are much larger than this could potentially slow down the performance. 
