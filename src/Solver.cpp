#include "Solver.hpp"

// statics

std::vector<std::pair<Face, Face>> Solver::allEdges = {
    {U,F}, {U,R}, {U,B}, {U,L},
    {D,F}, {D,R}, {D,B}, {D,L},
    {F,R}, {F,L}, {B,R}, {B,L}
};


// constructors / destructor

Solver::Solver() : _numberOfMoves(0), _algorithmName("No Algorithm Selected"),_timestamp(NULL){}

Solver::Solver(Cube *cube) : _numberOfMoves(0), _algorithmName("No Algorithm Selected"),_timestamp(NULL), _cube(cube){
    initEdgeCubiesMap();
}


Solver::Solver(const Solver &toCopy) : _numberOfMoves(toCopy._numberOfMoves), _algorithmName(toCopy._algorithmName), _timestamp(toCopy._timestamp), _cube(toCopy._cube){}
Solver::Solver(Solver &&toMove) : _numberOfMoves(std::move(toMove._numberOfMoves)), _algorithmName(std::move(toMove._algorithmName)), _timestamp(std::move(toMove._timestamp)), _cube(std::move(toMove._cube)){}
Solver::~Solver(){}

// operator overloads

Solver &Solver::operator=(const Solver &toCopy){
    if (&toCopy != this){
        this->_numberOfMoves = toCopy._numberOfMoves;
        this->_algorithmName = toCopy._algorithmName;
        this->_timestamp = toCopy._timestamp;
        this->_cube = toCopy._cube;
    }

    return (*this);
}

Solver &Solver::operator=(Solver &&toMove){
    if (&toMove != this){
        this->_numberOfMoves = std::move(toMove._numberOfMoves);
        this->_algorithmName = std::move(toMove._algorithmName);
        this->_timestamp = std::move(toMove._timestamp);
        this->_cube = std::move(toMove._cube);
    }

    return (*this);
}


// helper functions

LocalCoordinate getLocalInDirectionOfFace(const std::vector<int> &faceNormal, std::map<LocalCoordinate, std::vector<int>> faceLocalCoordinates){
    LocalCoordinate coordinate;
    // printVector(faceNormal);

    for (auto [local, localNormal] : faceLocalCoordinates){
        int product = doProduct(faceNormal, localNormal);
        if (product == 1){
            coordinate = local;
            break;
        }
    }

    return coordinate;
}



// private member functions


std::array<int, 2> Solver::computeCubesEdgeIndices(Face face1, Face face2){


    std::vector<int> face1Normal = _cube->getNormals()[face1];
    std::vector<int> face2Normal = _cube->getNormals()[face2];
    std::map<LocalCoordinate, std::vector<int>> face1LocalCoordinates = _cube->getLocalCoordinates()[face1];
    std::map<LocalCoordinate, std::vector<int>> face2LocalCoordinates = _cube->getLocalCoordinates()[face2];

    // printVector(face1LocalCoordinates[TOP]);

    std::array<int, 2> indices;

    indices[0] = _cube->getLocalCoordinatesIndices()[getLocalInDirectionOfFace(face2Normal, face1LocalCoordinates)][1] + _cube->faceStart(face1);;
    indices[1] = _cube->getLocalCoordinatesIndices()[getLocalInDirectionOfFace(face1Normal, face2LocalCoordinates)][1] + _cube->faceStart(face2);

    return indices;
}
std::array<Color, 2> Solver::computeCubesEdgeBaseColors(const std::array<int, 2> &indices){
    std::array<Color, 2> baseColors;

    baseColors[0] = _cube->getData()[indices[0]];
    baseColors[1] = _cube->getData()[indices[1]];

    return baseColors;
}

void Solver::initEdgeCubiesMap(){
    for (auto &edge : allEdges){
        std::array<int, 2> indices = computeCubesEdgeIndices(edge.first, edge.second);
        std::array<Color, 2> baseColors = computeCubesEdgeBaseColors(indices);
        _edgeCubiesMap[edge] = {baseColors, indices};
        // std::cout << "[" << indices[0] << "," << indices[1]  << "]";
        // std::cout << faceToStr(edge.first) << " :" << colorToStr(static_cast<Color>(_cube->getData()[indices[0]])) << " | " << faceToStr(edge.second) <<  " :" << colorToStr(static_cast<Color>(_cube->getData()[indices[1]])) << "\n";
    }
}

bool Solver::isEdgeFlipped(std::pair<Face, Face> edge){
    auto normals = _cube->getNormals();
    std::vector<int> solvedStateNormal1 = normals[edge.first];
    std::vector<int> solvedStateNormal2 = normals[edge.second];
    std::vector<int> solvedCross = crossProduct3(solvedStateNormal1, solvedStateNormal2);
    std::array<Color, 2> edgeBaseColor =  _edgeCubiesMap[edge].first;
    std::array<Color, 2> edgeBaseColorReversed =  {edgeBaseColor[1], edgeBaseColor[0]};

    for (auto &[currEdge, pair] : _edgeCubiesMap){
        std::array<int, 2> indices = pair.second;
        std::array<Color, 2> currentColors =  { _cube->getData()[indices[0]], _cube->getData()[indices[1]]} ;
        if ((currentColors == edgeBaseColor) || (currentColors == edgeBaseColorReversed))
            return (currentColors == edgeBaseColorReversed);
    }

    throw("Error, the cube itself is not good");
    return false;
}

// public member functions



int Solver::flippedEdgesHeuristic(){
    int counter = 0;
    for (auto &edge : allEdges){
        if (isEdgeFlipped(edge))
            counter++;
    }
    return  static_cast<int>(std::ceil(static_cast<double>(counter) / 4));
}

void Solver::solve(Cube &cube){
    (void)cube;
}


Cube *Solver::getCube() const
{
    return _cube;
}
