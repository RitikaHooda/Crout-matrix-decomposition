# Crout-matrix-decomposition
This shows how to perform crout matrix decomposition using serial and parallelised programs using threads(OpenMP) and processes(MPI). 
###### Inputs:
1. n: number of rows and columns of the square matrix
2. f: filename that contains an n*n matrix (A)
3. t: number of threads/processes
###### Outputs:
1. L.txt: contains lower triangular matrix after decomposition
2. U.txt: contains upper triangular matrix after decomposition

#### File structure
1. serial.cpp: serial program for performing crout matrix decomposition. 
2. p1.cpp: parallelised program using multiple threads with OpenMP. 
3. p2.cpp: parallelised program using multiple processes with MPI. 
