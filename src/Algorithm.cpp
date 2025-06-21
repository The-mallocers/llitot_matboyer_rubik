#include "Algorithm.hpp"


Algorithm::Algorithm(const std::string& name, Solver &solver, Cube &cube) : _name(name), _solver(solver), _cube(cube) {}
Algorithm::~Algorithm() {}

        
const std::string &Algorithm::getName() const{ 
    return this->_name;
}

