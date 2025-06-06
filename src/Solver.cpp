#include "Solver.hpp"


// constructors / destructor

Solver::Solver() : _numberOfMoves(0), _algorithmName("No Algorithm Selected"),_timestamp(NULL){}
Solver::Solver(const Solver &toCopy) : _numberOfMoves(toCopy._numberOfMoves), _algorithmName(toCopy._algorithmName), _timestamp(toCopy._timestamp), _originalCubeCopy(toCopy._originalCubeCopy){}
Solver::Solver(Solver &&toMove) : _numberOfMoves(std::move(toMove._numberOfMoves)), _algorithmName(std::move(toMove._algorithmName)), _timestamp(std::move(toMove._timestamp)), _originalCubeCopy(std::move(toMove._originalCubeCopy)){}
Solver::~Solver(){}

// operator overloads

Solver &Solver::operator=(const Solver &toCopy){
    if (&toCopy != this){
        this->_numberOfMoves = toCopy._numberOfMoves;
        this->_algorithmName = toCopy._algorithmName;
        this->_timestamp = toCopy._timestamp;
        this->_originalCubeCopy = toCopy._originalCubeCopy;
    }

    return (*this);
}

Solver &Solver::operator=(Solver &&toMove){
    if (&toMove != this){
        this->_numberOfMoves = std::move(toMove._numberOfMoves);
        this->_algorithmName = std::move(toMove._algorithmName);
        this->_timestamp = std::move(toMove._timestamp);
        this->_originalCubeCopy = std::move(toMove._originalCubeCopy);
    }

    return (*this);
}

// private member functions


// public member functions

void Solver::solve(Cube &cube){
    (void)cube;
}


const Cube &Solver::getOriginalCube() const
{
    return _originalCubeCopy;
}
