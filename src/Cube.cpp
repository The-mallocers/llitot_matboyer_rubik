#include "Cube.hpp"
#include <iostream>

// static declarations

std::vector<std::pair<Face, Face>> Cube::allEdges = {
    {U,F}, {U,R}, {U,B}, {U,L},
    {D,F}, {D,R}, {D,B}, {D,L},
    {F,R}, {F,L}, {B,R}, {B,L}
};


std::vector<Corner> Cube::allCorners = {
    UFL, UFR, UBR, UBL,
    DFR, DFL, DBL, DBR
};


std::map<Corner , std::array<Face, 3>> Cube::faceOfCorner = {
    {UFL, {U, F , L}},
    {UFR, {U, F , R}},
    {UBR, {U, B , R}},
    {UBL, {U, B , L}},
    {DFL, {D, F , L}},
    {DFR, {D, F , R}},
    {DBR, {D, B , R}},
    {DBL, {D, B , L}}
};

std::map<Edge, std::vector<Slice>> Cube::slicesOfEdges = {
    { UF, {SLICE_U, SLICE_F, SLICE_M}},
    { UR, {SLICE_U, SLICE_R, SLICE_S}},
    { UB, {SLICE_U, SLICE_B, SLICE_M}},
    { UL, {SLICE_U, SLICE_L, SLICE_S}},
    { DF, {SLICE_D, SLICE_F, SLICE_M}},
    { DR, {SLICE_D, SLICE_R, SLICE_S}},
    { DB, {SLICE_D, SLICE_B, SLICE_M}},
    { DL, {SLICE_D, SLICE_L, SLICE_S}},
    { FR, {SLICE_F, SLICE_R, SLICE_E}},
    { FL, {SLICE_F, SLICE_L, SLICE_E}},
    { BR, {SLICE_B, SLICE_R, SLICE_E}},
    { BL, {SLICE_B, SLICE_L, SLICE_E}}
};

std::map<Face, std::vector<Corner>> Cube::_relatedCorners = {
    {U, {UFL, UFR, UBR, UBL}},
    {D, {DFL, DFR, DBR, DBL}},
    {F, {UFL, UFR, DFR, DFL}},
    {B, {UBR, UBL, DBL, DBR}},
    {R, {UFR, UBR, DBR, DFR}},
    {L, {UBL, UFL, DFL, DBL}}
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

Cube::Cube(const Cube& toCopy) : _order(toCopy._order), _data(toCopy._data), _localCoordinates(toCopy._localCoordinates) , permutations(toCopy.permutations),  _baseEdgePositions(toCopy._baseEdgePositions), _relativeCornerDeltas(toCopy._relativeCornerDeltas), flipTracker(toCopy.flipTracker),twistTracker(toCopy.twistTracker), cornerPermutations(toCopy.cornerPermutations), baseCornerPositions(toCopy.baseCornerPositions){}
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
        // UDE_mapping[i] = i < 8 ? UD : E;
    }

    for (int i = 0; i < 8; i++){
        cornerPermutations[i] = i;
        baseCornerPositions[i] = i;
    }


    for (auto &[moveFace, relativeCorners] : _relatedCorners) {

        // std::cout <<"relativeCornerDeltas for face : " << faceToStr(moveFace) << std::endl;
        std::vector<std::pair<Corner, Delta>> relativeDeltas;

        for (unsigned i = 0; i < relativeCorners.size(); i++){
            Delta whatDelta = NULL_DELTA;

            if (moveFace == F || moveFace == L)
                whatDelta = i % 2 == 0 ? CW : CCW;
            if (moveFace == B || moveFace == R)
                whatDelta = i % 2 == 0 ? CCW : CW;

            relativeDeltas.push_back(std::make_pair(relativeCorners[i], whatDelta));
            // std::cout << "\t"<< cornerToStr(relativeDeltas[i].first) << " : "<< relativeDeltas[i].second << std::endl;

        }


        _relativeCornerDeltas[moveFace] = relativeDeltas;

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
    for (int times = 0; times < move.times; times++) {
        std::vector<unsigned> indicesOfAffectedEdges;
        for (auto& relatedFace : _relatedFaces[move.face])
            indicesOfAffectedEdges.push_back(indexOfEdge({move.face, relatedFace}));
        
        std::vector<unsigned> indicesOfAffectedEdgesCpy(indicesOfAffectedEdges);
        for (unsigned i = 0; i < indicesOfAffectedEdges.size(); ++i) {
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

            // std::cout << "Permutations after : " << move.face << move.direction << std::endl;

            // for (auto &perm : permutations){
            //     std::cout << perm << " ";
            // }
            // std::cout << std::endl;


        std::vector<unsigned> indicesOfRelatedCorners;
        for (auto& relatedCorner : _relatedCorners[move.face])
            indicesOfRelatedCorners.push_back(indexOfCorner(relatedCorner, allCorners));
        
        std::vector<unsigned> indicesOfRelatedCornersCpy(indicesOfRelatedCorners);
        for (unsigned i = 0; i < indicesOfRelatedCornersCpy.size(); ++i) {
            int newStateIndex = (i + move.direction + 4) % 4;
            indicesOfRelatedCornersCpy[newStateIndex] = indicesOfRelatedCorners[i];
        }

        std::array<unsigned, 8> tempCornerPermutations = cornerPermutations;
        std::array<unsigned, 8> tempTwistTracker = twistTracker;

        for (unsigned i = 0; i < indicesOfRelatedCorners.size(); ++i) {
            unsigned toSlot = indicesOfRelatedCorners[i];
            unsigned fromSlot = indicesOfRelatedCornersCpy[i];
            cornerPermutations[toSlot] = tempCornerPermutations[fromSlot];
        }

        bool affectsTwist = (move.face == F || move.face == B || move.face == R || move.face == L);

        for (unsigned i = 0; i < indicesOfRelatedCorners.size(); ++i) {
            unsigned toSlot = indicesOfRelatedCorners[i];
            unsigned fromSlot = indicesOfRelatedCornersCpy[i];
            unsigned pieceID = tempCornerPermutations[fromSlot];

            cornerPermutations[toSlot] = pieceID;
            if (affectsTwist) {
                int twistDelta = _relativeCornerDeltas[move.face][i].second;
                twistTracker[pieceID] = (tempTwistTracker[pieceID] + twistDelta) % 3;
            }
        }
    }
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

unsigned Cube::indexOfCorner(Corner corner, std::vector<Corner> slots){
    unsigned count = 0;
    for (auto &currentCorner : slots){
        if (currentCorner == corner)
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

std::array<unsigned, 8> Cube::getTwistTracker(){
    return twistTracker;
}

unsigned Cube::encodeEdgeOrientation() {
    unsigned key = 0;

    for (int i = 0; i < 11; ++i) {
        key <<= 1;
        key |= flipTracker[i];
    }
    return key;
}

int indexOfSlice(Slice toFind, std::vector<Slice> &in){
    int index = -1;

    for (unsigned i = 0; i < in.size(); i++){
        if (toFind == in[i] ){
            index = i;
            break;
        }

    }

    // std::cout << index << std::endl;
    return index;
}

int Cube::isEdgeOnMSlice(unsigned currentPosition) {
    //     for (auto &perm : permutations){
    //     std::cout << perm << " ";
    // }
    //     std::cout << std::endl;
    return indexOfSlice(SLICE_M, slicesOfEdges[static_cast<Edge>(currentPosition)]);
}

unsigned long Cube::encodeMissplacedEdges_SLICE_M() {
    unsigned key = 0;
    std::vector<unsigned> toCheck = {0,2,4,6};
    
    for (int i = 0; i < 4; ++i) {
        key <<= 1;
        unsigned pos = toCheck[i];

        if (isEdgeOnMSlice(permutations[pos]) != -1)
            key |= 1;
        else
            key |= 0;
    }
    
    return key;
}

unsigned long Cube::encodetwistedCorners(){
    return encodeTwistedCorners({0,1,2,3, 4,5, 6,7});
}

unsigned long Cube::encodeTwistedCorners(const std::vector<unsigned> &toCheck) {
    unsigned key = 0;

    for (auto &index : toCheck ){
        // key +=  twistTracker[index];
        // key *= 3;

        key = key * 3 + twistTracker[index];
    }
    return key;

}

