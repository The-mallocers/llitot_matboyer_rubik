#pragma once
#include "enums.hpp"
#include "enums.hpp"
#include <vector>
#include <iostream>

typedef struct s_move {
    Face face;
    Direction direction;
    int times;
} t_move ;

typedef struct s_rotation {
    std::vector<int> faceIndices;
    std::vector<std::vector<int>> edgesIndices;

    s_rotation rotate (Direction direction, unsigned cubeOrder){
        if (this->faceIndices.empty() || this->edgesIndices.size() < 4)
            throw std::invalid_argument("Invalid states ineither rotation.faceIndices or rotation.edgesIndices");

        s_rotation newState;

        newState.faceIndices.resize(9);
        newState.edgesIndices.resize(4);

        for (unsigned i = 0; i < this->faceIndices.size(); i++){
            int newStateIndex = getFaceRotationIndex(direction, cubeOrder, i);
            newState.faceIndices[newStateIndex] = this->faceIndices[i];
        }

        for (unsigned i = 0; i < this->edgesIndices.size() ; ++i) {
            int newStateIndex = (i + direction + 4) % 4;
            newState.edgesIndices[newStateIndex] = this->edgesIndices[i];
        }

        return newState;
    }



private:
    unsigned getFaceRotationIndex(Direction direction, unsigned cubeOrder, unsigned i){

        unsigned row = (i / cubeOrder);
        unsigned col = (i % cubeOrder);


        if (direction == CLOCK_WISE){
            int newRow = col;
            int newCol = cubeOrder - 1 - row;
            return ((newRow) * cubeOrder + (newCol));
        } else {
            int newRow = cubeOrder - 1 - col;
            int newCol = row;
            return ((newRow) * cubeOrder + (newCol));
        }
    }

} t_rotation ;