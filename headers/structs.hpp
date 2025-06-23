#pragma once
#include "enums.hpp"
#include "enums.hpp"
#include <vector>
#include <iostream>
// #include "Cube.hpp"

class Cube;
typedef struct s_move {
    Face face;
    Direction direction;
    int times;
} t_move ;

typedef struct s_rotation {
    Face rotatedFace;
    std::vector<int> faceIndices;
    std::vector<std::vector<int>> edgesIndices;

    s_rotation rotate (Direction direction, Cube &cube);

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