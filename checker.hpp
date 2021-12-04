#include "grypho.hpp"

#include <functional>
#include <random>
#include <chrono>
#include <iostream>

//function to check correctness of GI problem solution
//implementation of algorithm described by M. Blum and S. Kannan in [BK90]
//probability of wrong answer is 2 ^ (-repeat)
bool BK_checker(Graph<int> G, Graph<int> H, std::function< std::pair< bool, std::vector<int> >(Graph<int>, Graph<int>) > P, int repeat = 100);

