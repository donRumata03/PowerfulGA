# Powerful GA

This is a powerful C++ library, that implements Genetic Algorithm.
Why you should choose this one?

#### It supports these features:
 1) Optional Multithreading support for computing your fitness function. (as many threads as you want)
 2) Advanced matting system (There are special realistic distributions for genes while matting) 
 3) Flexibly configurable random and heuristics anywhere it\`s appropriate.
 4) Each generation consists of many different types of genomes 
 5) Fast normal distributed numbers generator.
 6) There were other measures arranged to improve performance (for example, distributing genome pointers instead of genomes themselves)

## Usage

 This library uses CMake as its build system.
 However, you\`ll probably have troubles with building this library, because it uses "pythonic" library from "https://github.com/donRumata03/pythonic", which I often modify and use for multiple projects.
 But to use it you can add cmake_build-release/GA.lib or cmake_build-debug/GA.lib (for debug mode) as static libraries, which are built by MSVC 2019 compiler, and header files in GA/.
 To explore the project there are also files .cpp in GA/src
 There is an example of using the library in Tests/main.cpp
 There are functions ga_optimize, which implements the main algorithm and log_ga_optimize, which is the wrapper, which logariphmates different parameters (you can choose them) and can also generate fitness function, which is nesessary for GA from loss function, which is usualy esasier to define. 

#### ga_optimize function takes following parameters:

	fitness_function ( std::function(double(std::vector<double>)) ) - it\`s actually the function to maximize

	variable_ranges ( std::vector<std::pair<double, double>> ) - Refers to the ranges of different variables, where you are going to find maximum. 
 
	params : ( type: GA_params ) : hyperparameters of the algorithm, which are necessary for genetic algorithms.
	This is the struct, specially defined to be passed to the function.

Here are its poles with description:
 1) population_size (type : size_t) - doesn\`t need description
 2) epoch_num (type : size_t) - also doesn\`t need description
 3) allow_multithreading (type : bool, default : false). If this parameter is set true, there are some threads created to compute your fitness_function (assume, that it takes the biggest amount of time)
 4) threads (type : size_t, default : (std\:\:\thread\:\:hardware_concurrency() - 2)). This parameter only makes sense if multithreading is allowed. It determines the number of additional threads used to compute fitness_function for all the genomes in each population.
 5) Hazing percent


## Implementation notes

This is not absolutely standard GA implementation. It\`s designed with assumption, that the majority of time used used for computing fitness_function.
That means, that almost the only goal for performance is to minimize the number of its computations.
In order to achieve the aim, different heuristics are used. 

To start with, there\`s one difference between this implementation and the standard one, which could seem to be suspicious to you. 
It is the fact, that the genomes are stored as vectors of floating-point values instead of usual binary genome representation.
This method is usually followed by low-quality code written by ...

But I\`ll try to explain why I do it this way. 
Genetic Algorithm is nothing more than a tool for function optimization.
That function must take floating-point parameters, which are continuous by their nature.
The initial reason to represent genes the binary way is that in nature there is the operation called crossover.
A point is chose randomly for crossover and the new genome is formed from two halves from different genome.
The idea is that this is the way how the child gets some features from booth its parents, what helps us to achieve the main aim.
Here we notice, that all the genes in genome are "crossovered" absolutely independently. 
#### "We mostly need to search in the places where the function is bigger" - this is the idea formalized in GAs.
In each iterations we use some genomes from the previous generation somehow and add the results of their matting to them.
So, the described method of crossover is OK. 
It pays respect to both parents and the possible result of matting for each gene is somehow distributed based on parents\` genes.
I declare, that the only advantage of this method over mine is that it might be a bit faster, but as far as it said above, it doesn\`t matter.
So, what is the method? Considering, that the genes in genomes are crossovered independently (which is, obviously, a good idea), we have the following task for each gene:
given 2 numbers and optionally, this variable\`s range, we should somehow chose that number for the child. 
The simplest way to do it is by returning the average between them. Obviously, it isn\`t the best idea. This method terribly decreases variance of genomes and makes algorithm diverge to local, not global maxima.
And, of course, it should be a non-deterministic operation, so, we need to form the function of probability density.
I tried out and thought about different functions and I came to conclusion, that it should be made of 3 normal probability density distributions built above the genomes.
One of them has it\`s peak in the point of one genome\`s value, the second - in the point of the next one and the last one finds itself in the middle point. 
Sigma values are proportional to the difference between the parents\` values. 
The coefficient is chosen experimentally from reasons of maintaining population variance from the one hand and fast convergence from the other one.
