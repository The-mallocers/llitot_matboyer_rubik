#include "Cube.hpp"
#include <iostream>

// static declarations
std::map<Face, std::vector<int>> Cube::normals = {
    {Face::U , {0, 1, 0}},
    {Face::D , {0, -1, 0}},
    {Face::R , {1, 0, 0}},
    {Face::L , {-1, 0, 0}},
    {Face::F , {0, 0, 1}},
    {Face::B , {0, 0, -1}},
};


// constructors and destructor
Cube::Cube() : _order(0) {
    this->_data.resize(6 * _order * _order);
    this->fill();
}
Cube::Cube(const int order) : _order(order) {
    this->_data.resize(6 * _order * _order);
    this->init();

}

Cube::Cube(const Cube& toCopy) : _order(toCopy._order), _data(toCopy._data){}
Cube::Cube(Cube&& toMove) noexcept : _order(std::move(toMove._order)), _data(std::move(toMove._data)){}
Cube::~Cube(){}

// operators overloading

Cube &Cube::operator=(const Cube& toCopy) {
    if (&toCopy != this){
        this->_order = toCopy._order;
        this->_data = toCopy._data;
    }
    return *this;
}

Cube &Cube::operator=(Cube&& toMove) noexcept{
    if (&toMove != this){
        this->_order = std::move(toMove._order);
        this->_data = std::move(toMove._data);
    }

    return *this;
}


// private memeber functions

void Cube::createFaceRelations() {
    for (unsigned i = 0; i < 6 ; ++i){
        const std::vector<int> &normal_1 = normals[static_cast<Face>(i)];
        std::vector<Face> relatedFaces;

        for (unsigned j = 0; j < 6 ; ++j) {
            if (i == j)
                continue;
            const std::vector<int> &normal_2 = normals[static_cast<Face>(j)];
            const std::vector<int> crossProduct = crossProduct3(normal_1, normal_2);

            int vectorSum = std::accumulate(crossProduct.begin(), crossProduct.end(), 0);
            if (vectorSum == 0)
                continue;

            relatedFaces.push_back(static_cast<Face>(j));
        }


        this->_relatedFaces[static_cast<Face>(i)] = relatedFaces;

    }

}


std::map<LocalCoordinate, std::vector<int>> Cube::findLocalCoordinates(Face face){
    std::map<LocalCoordinate, std::vector<int>> local;
    const std::vector<int> &faceNormal = normals[face];
    std::vector<int> refVector;

    if (faceNormal == std::vector<int>{0,1,0} || faceNormal == std::vector<int>{0, -1,0})
        refVector = {1,0,0};
    else
        refVector = {0,1,0};

    local[RIGHT] = crossProduct3(faceNormal, refVector);
    local[LEFT] = multiplyVector(local[RIGHT], -1);

    local[TOP] = crossProduct3(faceNormal, local[RIGHT]);
    local[DOWN] = multiplyVector(local[TOP], -1);

    return local;
}

void Cube::mapLocalCoordinates(){
    unsigned i = 0;
    while (i < 6){
        _localCoordinates[static_cast<Face>(i)] = findLocalCoordinates(static_cast<Face>(i));
        i++;
    }

    printLocalFaces(_localCoordinates);
}

void Cube::fill() {
    for (unsigned faceCount = 0 ; faceCount < 6; faceCount++){
        for (unsigned y = 0 ; y < _order; y++){
            for (unsigned x = 0 ; x < _order; x++){
                this->_data[(faceCount * (_order * _order)) + y * _order + x] = static_cast<Color>(faceCount);
            }
        }

        faceStart(static_cast<Face>(faceCount));
        faceEnd(static_cast<Face>(faceCount));
    }
}

void Cube::init() {
    this->createFaceRelations();
    this->mapLocalCoordinates();
    this->fill();
}

unsigned Cube::faceStart(Face face){
    unsigned i = 0;
    unsigned faceStart = 0;
    while (static_cast<Face>(i) != face) {
        faceStart += (_order * _order);
        i++;
    }
    return faceStart;
}

unsigned Cube::faceEnd(Face face){
    unsigned i = 0;
    unsigned faceEnd = (_order * _order) - 1;
    while (static_cast<Face>(i) != face) {
        faceEnd += (_order * _order);
        i++;
    }
    return faceEnd;
}


bool Cube::isSolved() const {
    // TODO
    return false;
}


void Cube::applyMove(t_move move) {
    // TODO
    (void)move;
}

// public member functions

void Cube::applyMoves(std::vector<t_move> moves){
    // TODO
    (void)moves;
}

const std::vector<Color> Cube::getData() const {
    return _data;
}
