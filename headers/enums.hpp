#pragma once 

enum Face {
    U,
    D,
    F,
    B,
    L,
    R,
    NULL_FACE
};



enum Corner {
    UFL,
    UFR,
    UBR,
    UBL,
    DFL,
    DFR,
    DBR,
    DBL,
};

enum Edge {
    UF,
    UR,
    UB,
    UL,
    DF,
    DR,
    DB,
    DL,
    FR,
    FL,
    BR,
    BL
};



enum Delta {
    NULL_DELTA = 0,
    CW = 1,
    CCW = 2
};


enum Direction {
    ANTI_CLOCK_WISE = -1,
    CLOCK_WISE = 1
};

enum LocalCoordinate {
    TOP,
    RIGHT,
    DOWN,
    LEFT,
};

enum Color {
    WHITE,
    YELLOW,
    GREEN,
    BLUE,
    ORANGE,
    RED,
};

enum Slice {
    SLICE_E,
    SLICE_M,
    SLICE_S,
    SLICE_U,
    SLICE_D,
    SLICE_B,
    SLICE_F,
    SLICE_R,
    SLICE_L,
    UD,
};