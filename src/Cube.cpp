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

void Cube::init() {
    this->createFaceRelations();
    this->fill();
}

void Cube::fill() {
    for (unsigned faceCount = 0 ; faceCount < 6; faceCount++){
        for (unsigned y = 0 ; y < _order; y++){
            for (unsigned x = 0 ; x < _order; x++){
                this->_data[(faceCount * (_order * _order)) + y * _order + x] = static_cast<Color>(faceCount);
            }
        }
    }
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
