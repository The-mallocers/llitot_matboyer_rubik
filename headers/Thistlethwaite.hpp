#pragma once
#include "Algorithm.hpp"
#include "Solver.hpp"


#define FOUND -1
#define TOO_EXPENSIVE -2

class Thistlethwaite : public Algorithm {
    private:
        void G0();
        void G1();
        void G2();
        void G3();

        const std::vector<t_move> SequenceG0;
        int IDAStar();

    public:
        Thistlethwaite(const Solver &solver);
        const std::vector<t_move> getSequence() override;
        ~Thistlethwaite() override;
};


