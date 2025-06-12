#pragma once
#include "enums.hpp"
#include "structs.hpp"
#include <vector>
#include <map>
#include <numeric>
#include "math.hpp"
#include "utils/printing.hpp"

class Cube
{
    private:
        unsigned _order;
        std::vector<Color> _data;
        void printFace(Face face);
        void print();
        static std::map<Face, std::vector<Face>> _relatedFaces;
        std::map<Face, std::map<LocalCoordinate, std::vector<int>>> _localCoordinates;


        unsigned faceStart(Face face);
        unsigned faceEnd(Face face);
        void createFaceRelations();
        std::vector<std::vector<int>> getFaceEdges(Face face);
        t_rotation encodeRotation(t_move move);
        std::map<LocalCoordinate, std::vector<int>> findLocalCoordinates(Face face);
        void mapLocalCoordinates();
        void init();
        void fill();
        bool isSolved() const;
        void applyMove(t_move move);
    public:
        Cube();
        Cube(const int order);
        Cube(const Cube& toCopy);
        Cube(Cube&& toMove) noexcept;
        ~Cube();

        Cube &operator=(const Cube& toCopy);
        Cube &operator=(Cube&& toMove) noexcept;

        void applyMoves(std::vector<t_move> moves);
        const std::vector<Color> getData() const;

        static std::map<Face, std::vector<int>> normals;
        static std::map<LocalCoordinate, std::vector<int>> localCoordinatesIndices;
};
