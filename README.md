# Roadef2020

Source Code Team 56 Roadef 2020

Universidad de Castilla-La Mancha Universidad de Valencia

Francisco Parreno, Ramón Álvarez-Valdés and Consuelo Parreno-Torres

We used -fopenmp and standard c++ and CPLEX

The makefile is for CPLEX 20.1, if you have a recent version, change that lines in the makefile and the libraries

You should put all the files in the same folder than the makefile. 

If you want to use, use the standard parameters of the challenge are indicated in:

https://www.roadef.org/challenge/2020/en/finalSubmission.php

You can also see the solutions of the set C

Options in the script. 

#!/bin/bash 

#SBATCH -c 2 --mem=16GB

#SBATCH --ntasks=1

#SBATCH --cpus-per-task=2

export OMP_NUM_THREADS=4
