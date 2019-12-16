# Parallel-Matrix-Fast-Exponentiation-Algorithm
Parallel implmentation of the Matrix fast exponentiation algorithm using MPI which is used to calculate large powers of a matrix. The details of the algorithm are listed in the report.pdf

<h2> Dependencies </h2>

<ul>
  <li>gcc-5.4.0</li>
  <li>openmpi-1.10.2</li>
</ul>

<h2> Usage </h2>

Clone the repository using:
```console
foo@bar:~$ git clone git@github.com:neeldani/Parallel-Matrix-Fast-Exponentiation-Algorithm.git
```

Navigate to the project directory:
```console
foo@bar:~$ cd Parallel-Matrix-Fast-Exponentiation-Algorithm
```
Execute the run.sh file which will compile and run the master.c and slave.c programs
```console
foo@bar:~$ chmod a+x run.sh
foo@bar:~$ ./run.sh
```

<h2> Future Works </h2>

Currently, the code is functional only for matrices having number of rows divisible by number of slaves (both can be configured in the config.h file). Future work include generalization of the algorithm by removing the above mentioned constraint by using Scatterv and Gatherv functions instead of Scatter and Gather respectively.  

<h2> References </h2>

The project is based on the idea covered in the paper *A parallel algorithm for matrix fast exponentiation based on MPI* published in the *2018 IEEE 3rd International Conference on Big Data Analysis (ICBDA)*.


