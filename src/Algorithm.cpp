#include "Algorithm.hpp"

const std::string &Algorithm::getName() const{
    return this->_name;
}

Algorithm::Algorithm(const std::string& name) : _name(name) {
}

Algorithm::~Algorithm() {
}
