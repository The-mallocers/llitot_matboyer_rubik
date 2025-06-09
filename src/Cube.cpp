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

std::map<LocalCoordinate, std::vector<int>> Cube::localCoordinatesIndices = {
    {LocalCoordinate::TOP,    {0,1,2}},
    {LocalCoordinate::DOWN,   {6,7,8}},
    {LocalCoordinate::LEFT,   {0,3,6}},
    {LocalCoordinate::RIGHT,  {2,5,8}},
};

std::map<Face, std::vector<Face>> Cube::_relatedFaces = {
    {F, {U, R, D, L}},
    {B, {U, L, D, R}},
    {U, {B, R, F, L}},
    {D, {F, R, B, L}},
    {R, {U, B, D, F}},
    {L, {U, F, D, B}},
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

// void Cube::createFaceRelations() {
//     for (unsigned i = 0; i < 6 ; ++i){
//         const std::vector<int> &normal_1 = normals[static_cast<Face>(i)];
//         std::vector<Face> relatedFaces;

//         for (unsigned j = 0; j < 6 ; ++j) {
//             if (i == j)
//                 continue;
//             const std::vector<int> &normal_2 = normals[static_cast<Face>(j)];
//             const std::vector<int> crossProduct = crossProduct3(normal_1, normal_2);

//             int vectorSum = std::accumulate(crossProduct.begin(), crossProduct.end(), 0);
//             if (vectorSum == 0 || crossProduct == multiplyVector(normal_1, -1))
//                 continue;

//             relatedFaces.push_back(static_cast<Face>(j));
//         }


//         this->_relatedFaces[static_cast<Face>(i)] = relatedFaces;
//     }

// }


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

    // printLocalFaces(_localCoordinates);
}

std::vector<std::vector<int>> Cube::getFaceEdges(Face face){
    std::vector<int> faceNormal = Cube::normals[face];
    std::vector<std::vector<int>> edgesCoordinates;

    std::vector<Face> relatedFaces = _relatedFaces[face];

    for (auto &relatedFace : relatedFaces){
        auto &locals = _localCoordinates[relatedFace];
        for (auto &[key, coordinates] : locals){
            if (doProduct(faceNormal, coordinates) == 1){
                edgesCoordinates.push_back(addIntToVector(localCoordinatesIndices[key], faceStart(relatedFace)));
                break;
            }
            continue;
        }
    }

    return edgesCoordinates;
}

t_rotation Cube::encodeRotation(t_move move){
    t_rotation rotation;
    unsigned i = faceStart(move.face);

    while (i <= faceEnd(move.face)){
        rotation.faceIndices.push_back(i);
        i++;
    }

    rotation.edgesIndices = getFaceEdges(move.face);

    return rotation;
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

void Cube::init() {
    // this->createFaceRelations();
    this->mapLocalCoordinates();
    this->fill();

    // std::cout << ">---------1--------<" << std::endl;

    // this->print();
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

    for (int times = 0 ; times < move.times; times++){
        t_rotation originalState = encodeRotation(move);
            t_rotation postRotationState = originalState.rotate(move.direction);
            const std::vector<Color> originalCubeData(_data);
            // std::cout << ">---------1--------<" << std::endl;
            // printVector(originalState.faceIndices);
            // for (auto &edge : originalState.edgesIndices)
            //     printVector(edge);

            // std::cout << ">---------2--------<" << std::endl;

            // printVector(postRotationState.faceIndices);
            // for (auto &edge : postRotationState.edgesIndices){
            //     printVector(edge);
            //     for (auto &indice : edge)
            //         _data
            // }


            for (unsigned i = 0; i < 4 ; i++){
                for (unsigned j = 0; j < 3 ; j++){
                    int oldValueIndex = originalState.edgesIndices[i][j];
                    int newValueIndex = postRotationState.edgesIndices[i][j];

                    _data[oldValueIndex] = originalCubeData[newValueIndex];
                }
            }

        }
        // print();
    

}

void Cube::print(){
    for (unsigned i = 0; i < 6; i++){
        std::cout << faceToStr(static_cast<Face>(i)) << " :\n" ;
        for (unsigned j = 0; j < 3; j++){
            for (unsigned k = 0; k < 3; k++){
                std::cout << _data[(i * (_order * _order)) + j * _order + k] ;
            }
            std::cout  << std::endl;
        }
    }
}

// public member functions

void Cube::applyMoves(std::vector<t_move> moves){
    // (void) moves;
    for (auto &move : moves) {
        applyMove(move);
    }
}

const std::vector<Color> Cube::getData() const {
    return _data;
}
