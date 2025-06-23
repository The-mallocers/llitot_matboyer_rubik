#include "structs.hpp"
#include "Cube.hpp"

s_rotation s_rotation::rotate (Direction direction, Cube &cube){
    if (this->faceIndices.empty() || this->edgesIndices.size() < 4)
        throw std::invalid_argument("Invalid states ineither rotation.faceIndices or rotation.edgesIndices");

    s_rotation newState;

    newState.faceIndices.resize(9);
    newState.edgesIndices.resize(4);
    
    //////

    for (unsigned i = 0; i < this->faceIndices.size(); i++){
        int newStateIndex = getFaceRotationIndex(direction, cube.getOrder(), i);
        newState.faceIndices[newStateIndex] = this->faceIndices[i];
    }
    for (unsigned i = 0; i < this->edgesIndices.size() ; ++i) {
        int newStateIndex = (i + direction + 4) % 4;
        newState.edgesIndices[newStateIndex] = this->edgesIndices[i];
    }

    return newState;
}