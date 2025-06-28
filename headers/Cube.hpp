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
        void buildCornerMoveTables();

        std::map<Face, std::vector<Face>> getRelatedFaces();
        std::map<Face, std::vector<int>> getNormals();
        std::map<LocalCoordinate, std::vector<int>> getLocalCoordinatesIndices();
        std::map<Face, std::map<LocalCoordinate, std::vector<int>>> getLocalCoordinates();
        unsigned getOrder() const;
        unsigned faceStart(Face face);
        unsigned faceEnd(Face face);
        Face getFaceFromIndex(unsigned index);
        unsigned indexOfEdge(const std::pair<Face, Face> &toFind);
        unsigned indexOfCorner(Corner corner, std::vector<Corner> slots);
        std::array<bool, 12> getFlipTracker();

        static std::vector<std::pair<Face, Face>> allEdges;
        static std::vector<Corner> allCorners;
        static std::map<Face, std::vector<Corner>> _relatedCorners;
        static std::map<Corner , std::array<Face, 3>> faceOfCorner;
        static std::map<Edge, std::vector<Slice>> slicesOfEdges;

        std::map<Face, std::vector<std::pair<Corner, Delta>>> _relativeCornerDeltas;
        // std::map<unsigned, Slice> UDE_mapping;
        std::array<bool, 12> flipTracker{};
        std::array<unsigned, 8> twistTracker{};
        std::array<unsigned,8> cornerPermutations{};
        std::array<unsigned,8> baseCornerPositions{};
        int computeCornerDelta(Corner corner, t_move &move);
        int computeCornerDelta(Corner corner, t_move& move, unsigned fromPos, unsigned toPos);
        int isEdgeOnMSlice( unsigned currentPosition);
        std::array<unsigned, 12> getBaseEdgePosition();
        std::array<unsigned, 12> getPermutations();
        std::array<unsigned, 8> getTwistTracker();
        unsigned encodeEdgeOrientation();
        // unsigned encodeMissplacedEdges_SLICE_M_MIDDLE();
        unsigned long encodetwistedCorners();
        unsigned long encodeMissplacedEdges_SLICE_M();
        unsigned long encodeTwistedCorners(const std::vector<unsigned> &toCheck);

};
