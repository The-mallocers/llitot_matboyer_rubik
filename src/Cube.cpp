#include "Cube.hpp"
#include <iostream>

// static declarations

std::vector<std::pair<Face, Face>> Cube::allEdges = {
    {U,F}, {U,R}, {U,B}, {U,L},
    {D,F}, {D,R}, {D,B}, {D,L},
    {F,R}, {F,L}, {B,R}, {B,L}
};

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
    // {F, {U, R, D, L}},
    // {B, {U, R, D, L}},  // reversed from {U, L, D, R} to match clockwise rotation
    // {U, {B, R, F, L}},
    // {D, {F, R, B, L}},
    // {R, {U, F, D, B}},
    // {L, {U, F, D, B}},  // reversed from {U, B, D, F}

    {F, {U, R, D, L}},  // Front face: Up, Right, Down, Left (clockwise)
    {B, {U, L, D, R}},  // Back face: Up, Left, Down, Right (clockwise)
    {U, {B, R, F, L}},  // Up face: Back, Right, Front, Left (clockwise)
    {D, {F, R, B, L}},  // Down face: Front, Right, Back, Left (clockwise)
    {R, {U, B, D, F}},  // Right face: Up, Back, Down, Front (clockwise)
    {L, {U, F, D, B}},  // Left face: Up, Front, Down, Back (clockwise)
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

Cube::Cube(const Cube& toCopy) : _order(toCopy._order), _data(toCopy._data), _localCoordinates(toCopy._localCoordinates) , permutations(toCopy.permutations),  _baseEdgePositions(toCopy._baseEdgePositions),  UDE_mapping(toCopy.UDE_mapping), flipTracker(toCopy.flipTracker){}
Cube::Cube(Cube&& toMove) noexcept : _order(std::move(toMove._order)), _data(std::move(toMove._data)){}
Cube::~Cube(){}

// operators overloading

Cube &Cube::operator=(const Cube& toCopy) {
    if (&toCopy != this){
        this->_order = toCopy._order;
        this->_data = toCopy._data;
        this->_localCoordinates = toCopy._localCoordinates;
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
        std::cout << "lalalala" << std::endl;
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

    rotation.rotatedFace = move.face;
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

    for (int i = 0 ; i < 12 ; i++){
        permutations[i] = i;
        _baseEdgePositions[i] = i;
        UDE_mapping[i] = i < 8 ? UD : E;
    }
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
//     std::cout << "=== R MOVE DEBUG ===" << std::endl;
// std::cout << "Related faces for R: ";
// for (auto face : _relatedFaces[move.face]) {
//     std::cout << faceToStr(face) << " ";
// }
// std::cout << std::endl;

    /////////////


// Apply the move logic...


    for (int times = 0 ; times < move.times; times++){
        std::vector<unsigned> indicesOfAffectedEdges;

        for (auto& relatedFace : _relatedFaces[move.face])
            indicesOfAffectedEdges.push_back(indexOfEdge({move.face, relatedFace}));

        std::vector<unsigned> indicesOfAffectedEdgesCpy(indicesOfAffectedEdges);
        for (unsigned i = 0; i < indicesOfAffectedEdges.size() ; ++i) {
            int newStateIndex = (i + move.direction + 4) % 4;
            indicesOfAffectedEdgesCpy[newStateIndex] = indicesOfAffectedEdges[i];
        }

        std::array<unsigned, 12> tempPermutations = permutations;
        std::array<bool, 12> tempFlipTracker = flipTracker;
        for (unsigned i = 0; i < indicesOfAffectedEdges.size(); ++i) {
            unsigned to = indicesOfAffectedEdges[i];
            unsigned from = indicesOfAffectedEdgesCpy[i];
            permutations[to] = tempPermutations[from];

            if (move.face == F || move.face == B)
                flipTracker[to] = !tempFlipTracker[from];
            else
                flipTracker[to] = tempFlipTracker[from]; 
        }

        /////////////// commented out because it's making everything slow
        // t_rotation originalState = encodeRotation(move);
        //     t_rotation postRotationState = originalState.rotate(move.direction, *this);
        //     const std::vector<Color> originalCubeData(_data);

        //     for (unsigned i = 0; i < 9; i++){
        //         int oldValueIndex = originalState.faceIndices[i];
        //         int newValueIndex = postRotationState.faceIndices[i];

        //         _data[oldValueIndex] = originalCubeData[newValueIndex];
        //     }
        //     for (unsigned i = 0; i < 4 ; i++){
        //         for (unsigned j = 0; j < 3 ; j++){
        //             int oldValueIndex = originalState.edgesIndices[i][j];
        //             int newValueIndex = postRotationState.edgesIndices[i][j];

        //             _data[oldValueIndex] = originalCubeData[newValueIndex];
        //         }
        //     }

        }

    // std::cout << ">---------2--------<" << std::endl;
    // print();

}


void Cube::printFace(Face face){

    unsigned start = faceStart(face);
    unsigned end = faceEnd(face);

    std::cout << faceToStr(face) << " :" << std::endl;

    unsigned count = 0;
    while (start <= end){
        if (count % 3 == 0)
            std::cout << std::endl;
        std::cout << colorToStr(static_cast<Color>(_data[start])) << " ";
        count++;
        start++;
    }

    std::cout << std::endl;

}

void Cube::print(){
    for (unsigned i = 0; i < 6; i++){
        printFace(static_cast<Face>(i));
    }
}

// public member functions

void Cube::applyMoves(std::vector<t_move> moves){
    // (void) moves;
    for (auto &move : moves) {
        applyMove(move);
    }
}
std::map<Face, std::vector<Face>> Cube::getRelatedFaces(){return _relatedFaces;}
std::map<Face, std::vector<int>> Cube::getNormals(){return normals;}
std::map<LocalCoordinate, std::vector<int>> Cube::getLocalCoordinatesIndices(){return localCoordinatesIndices;}
std::map<Face, std::map<LocalCoordinate, std::vector<int>>> Cube::getLocalCoordinates(){return _localCoordinates;}


const std::vector<Color> Cube::getData() const {
    return _data;
}

unsigned Cube::getOrder() const {
    return _order;
}


Face Cube::getFaceFromIndex(unsigned index){
    unsigned faceCounter  = 0;

    for (unsigned i = 0; i < _data.size(); i += 9){
        if (i <= index && index < i + 9){
            return static_cast<Face>(faceCounter);
        }
        faceCounter++;
    }

    throw("No face was found");
}

void Cube::undo(t_move move){
    if (move.times == 2){
        this->applyMove(move);
        return;
    }
    move.direction =  static_cast<Direction>(move.direction * -1);
    this->applyMove(move);
}

unsigned Cube::indexOfEdge(const std::pair<Face, Face> &toFind){
    unsigned count = 0;
    for (auto &edge : allEdges){
        if (edge == toFind || edge == std::pair<Face, Face>{toFind.second, toFind.first})
            break;
        count++;
    }

    return count;
}

std::array<bool, 12> Cube::getFlipTracker(){
    return flipTracker;
}

std::array<unsigned, 12> Cube::getBaseEdgePosition(){
    return _baseEdgePositions;
}

std::array<unsigned, 12> Cube::getPermutations(){
    return permutations;
}
int Cube::encodeEdgeOrientation() {
    int key = 0;

    for (int i = 0; i < 11; ++i) {
        key <<= 1;
        key |= flipTracker[i];
    }
    return key;
}


int Cube::encodeMissplacedEdgesEDU() {
    int key = 0;

    for (int i = 0; i < 12; ++i) {
        key <<= 1;
        key |= UDE_mapping[_baseEdgePositions[i]] != UDE_mapping[permutations[i]];
    }
    return key;
}
