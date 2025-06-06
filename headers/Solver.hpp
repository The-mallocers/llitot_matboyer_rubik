#pragma once
#include "enums.hpp"
#include "structs.hpp"
#include "Cube.hpp"
#include "Algorithm.hpp"
#include <string>
#include <vector>
#include <ctime>

// Getter for the original cube's copy.

class Solver {
    private:
        int _numberOfMoves;
        std::string _algorithmName;
        std::vector<t_move> _solveSequence;
        time_t _timestamp;
        Cube _originalCubeCopy;

        Algorithm *findAlgorithm(const Cube &cube); // not initializing this one in it's file because the flags will not let me compile.

    public:
        Solver();
        Solver(const Solver& toCopy);
        Solver(Solver&& toMove);
        ~Solver();


        Solver &operator=(const Solver& toCopy);
        Solver &operator=(Solver&& toMove);

        void solve(Cube &cube);
        const Cube &getOriginalCube() const;

};
