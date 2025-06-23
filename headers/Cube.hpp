#pragma once
#include "enums.hpp"
#include "structs.hpp"
#include <vector>
#include <map>
#include <numeric>
#include "math.hpp"
#include "utils/printing.hpp"

// typedef struct s_move t_move;
// typedef struct s_rotation t_rotation;
class Cube
{
    private:
        unsigned _order;
        std::vector<Color> _data;
        void printFace(Face face);
        void print();
        static std::map<Face, std::vector<Face>> _relatedFaces;
        static std::map<Face, std::vector<int>> normals;
        static std::map<LocalCoordinate, std::vector<int>> localCoordinatesIndices;
        std::map<Face, std::map<LocalCoordinate, std::vector<int>>> _localCoordinates;
        std::array<unsigned, 12> permutations{};
        std::array<unsigned, 12> _baseEdgePositions{};
        void createFaceRelations();
        std::vector<std::vector<int>> getFaceEdges(Face face);
        t_rotation encodeRotation(t_move move);
        std::map<LocalCoordinate, std::vector<int>> findLocalCoordinates(Face face);
        void mapLocalCoordinates();
        void init();
        void fill();
        bool isSolved() const;
    public:
        Cube();
        Cube(const int order);
        Cube(const Cube& toCopy);
        Cube(Cube&& toMove) noexcept;
        ~Cube();

        Cube &operator=(const Cube& toCopy);
        Cube &operator=(Cube&& toMove) noexcept;
        void undo(t_move move);
        void applyMove(t_move move);
        void applyMoves(std::vector<t_move> moves);
        const std::vector<Color> getData() const;


        std::map<Face, std::vector<Face>> getRelatedFaces();
        std::map<Face, std::vector<int>> getNormals();
        std::map<LocalCoordinate, std::vector<int>> getLocalCoordinatesIndices();
        std::map<Face, std::map<LocalCoordinate, std::vector<int>>> getLocalCoordinates();
        unsigned getOrder() const;
        unsigned faceStart(Face face);
        unsigned faceEnd(Face face);
        Face getFaceFromIndex(unsigned index);
        unsigned indexOfEdge(const std::pair<Face, Face> &toFind);

        std::array<bool, 12> getFlipTracker();

        static std::vector<std::pair<Face, Face>> allEdges;
        std::map<unsigned, Slice> UDE_mapping;
        std::array<bool, 12> flipTracker{};
        std::array<unsigned, 12> getBaseEdgePosition();
        std::array<unsigned, 12> getPermutations();
        int encodeEdgeOrientation();
        int encodeMissplacedEdgesEDU();


};
