#pragma once
#include "Algorithm.hpp"
#include "Parser.hpp"
#include "structs.hpp"
#include "enums.hpp"
#include "printing.hpp"
#include "Cube.hpp"
#include "Solver.hpp"
#include <functional>
#include <set>



// #define FOUND -1
// #define ALREADY_VISITED -2

// class Solver;

class Thistlethwaite : public Algorithm {
    private:
        int totalIDAStarCalls;
        std::vector<t_move> _allowedMoves;
        std::function<std::pair<int, int>(Cube&)> _currentHeuristic;
        std::function<std::pair<unsigned long, unsigned long>(Cube&)> _cachingCondition;

        std::vector<std::function<int(Cube&)>> _previousHeuristics;

        const std::vector<t_move> G0();
        const std::vector<t_move> G1();
        const std::vector<t_move> G2();
        void G3();

        const std::vector<t_move> SequenceG0;
        void IDAStarBaseIteration(std::vector<t_move> &sequence);
        std::pair<int,int> IDAStar(unsigned depth, std::pair<int, int> &limit, Cube &cube, std::vector<t_move> &sequence, Face lastMove,  std::set<std::pair<unsigned long, unsigned long>> &visited);
        bool breaksPreviousPhases(Cube& cube);
    public:
        Thistlethwaite(Solver &solver, Cube &cube);
        const std::vector<t_move> getSequence() override;
        ~Thistlethwaite() override;
};


