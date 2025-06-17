#include "Algorithm.hpp"


Algorithm::Algorithm(const std::string& name, const Solver &solver) : _name(name), _solver(solver) {}
Algorithm::~Algorithm() {}

const std::string &Algorithm::getName() const{
    return this->_name;
}
