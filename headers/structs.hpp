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
} t_rotation ;