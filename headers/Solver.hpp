#pragma once
#include "enums.hpp"
#include "structs.hpp"
#include "Cube.hpp"
#include "Algorithm.hpp"
#include "math.hpp"
#include <string>
#include <vector>
#include <ctime>
#include <cmath>

class Algorithm;
class Solver {
    private:

        int _numberOfMoves;
        std::string _algorithmName;
        std::vector<t_move> _solveSequence;
        time_t _timestamp;
        Cube *_cube;

        std::array<int, 2> computeCubesEdgeIndices(Face face1, Face face2);
        std::array<Color, 2> computeCubesEdgeBaseColors(const std::array<int, 2> &indices);
        static std::vector<std::pair<Face, Face>> allEdges;
        std::map<std::pair<Face, Face>, std::pair<std::array<Color, 2> , std::array<int, 2>>> _edgeCubiesMap;



        void initEdgeCubiesMap();
        
        Algorithm *findAlgorithm(const Cube &cube); // not initializing this one in it's file because the flags will not let me compile.
        bool isEdgeFlipped(std::pair<Face, Face> edge);
        
    public:
        Solver();
        Solver(Cube *cube);
        Solver(const Solver& toCopy);
        Solver(Solver&& toMove);
        ~Solver();
        
        
        Solver &operator=(const Solver& toCopy);
        Solver &operator=(Solver&& toMove);
        
        int flippedEdgesHeuristic();
        void solve(Cube &cube);
        Cube *getCube() const;

};
