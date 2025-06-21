#pragma once
#include "Algorithm.hpp"
#include "Parser.hpp"
#include "structs.hpp"
#include "enums.hpp"
#include "printing.hpp"
#include "Cube.hpp"
#include "Solver.hpp"
#include <functional>



#define FOUND -1
#define TOO_EXPENSIVE -2

// class Solver;

class Thistlethwaite : public Algorithm {
    private:

        std::vector<t_move> _allowedMoves;
        std::function<int(Cube&)> _currentHeuristic;
        const std::vector<t_move> G0();
        void G1();
        void G2();
        void G3();

        const std::vector<t_move> SequenceG0;
        int IDAStar(unsigned depth, unsigned limit, Cube &cube, std::vector<t_move> &sequence, Face lastMove);

    public:
        Thistlethwaite(Solver &solver, Cube &cube);
        const std::vector<t_move> getSequence() override;
        ~Thistlethwaite() override;
};


