#pragma once
#include "enums.hpp"
#include "enums.hpp"
#include <vector>

typedef struct s_move {
    Face face;
    Direction direction;
    int times;
} t_move ;

typedef struct s_rotation {
    std::vector<int> faceIndices;
    std::vector<std::vector<int>> edgesIndices;

    s_rotation rotate (Direction direction){
        if (this->faceIndices.empty() || this->edgesIndices.size() < 4)
            throw std::invalid_argument("Invalid states ineither rotation.faceIndices or rotation.edgesIndices");

        s_rotation newState;

        newState.edgesIndices.resize(4);


        for (unsigned i = 0; i < this->edgesIndices.size() ; ++i) {

            int newStateIndex = (i + direction + 4) % 4;
            // direction is either clockwise or anti clockwise enum
            newState.edgesIndices[newStateIndex] = this->edgesIndices[i];
        }

        // didn't implement it for now so :
        newState.faceIndices = this->faceIndices;

        return newState;
    }
} t_rotation ;