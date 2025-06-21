#pragma once
#include <string>
#include <vector>
#include "enums.hpp"
#include "structs.hpp"
#include "Cube.hpp"
// #include "Solver.hpp"

class Solver;
class Algorithm
{
    protected:
        const std::string _name;
        Solver &_solver;
        Cube &_cube;
        Algorithm(const std::string& name, Solver &solver, Cube &_cube);
    public:
        Algorithm() = delete;
        Algorithm(const Algorithm& toCopy) = delete;
        Algorithm(Algorithm&& toCopy) = delete;
        Algorithm& operator=(const Algorithm& toCopy) = delete;
        Algorithm& operator=(Algorithm&& toMove) noexcept = delete;
        virtual ~Algorithm();

        virtual const std::vector<t_move> getSequence() = 0;
        const std::string &getName() const;
};

