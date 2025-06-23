#include "Thistlethwaite.hpp"

Thistlethwaite::Thistlethwaite(Solver&solver, Cube &cube) : Algorithm("Thistlethwaite", solver, cube){
}






const std::vector<t_move>Thistlethwaite::G0(){
    Parser parser("F B R L U D F2 B2 R2 L2 U2 D2 F' B' R' L' U' D'");
    _allowedMoves = parser.getMoves();
    _currentHeuristic = [&](Cube &cube) { return _solver.flippedEdgesHeuristic(cube);};
    _cachingCondition = [&](Cube &cube) { return cube.encodeEdgeOrientation();};


    std::vector<t_move> sequence;
    IDAStarBaseIteration(sequence);

    std::cout << "----------------found G0 solution-----------------------" << std::endl;
    for (auto move : sequence){
        std::cout << faceToStr(move.face) << " " << move.direction << " " << move.times << "\n";
        _cube.applyMove(move);
    }
    return sequence;
}


const std::vector<t_move>Thistlethwaite::G1(){
    Parser parser("U D F2 B2 R2 L2 U2 D2 U' D'");
    _allowedMoves = parser.getMoves();
    // _currentHeuristic = [&](Cube &cube) { return _solver.flippedEdgesHeuristic(cube);}; // i will need to add G1 heuristic  here
    // _cachingCondition = [&](Cube &cube) { return _solver.flippedEdgesHeuristic(cube);}; // i will need to add G1 caching logic here

    std::vector<t_move> sequence;
    IDAStarBaseIteration(sequence);

    std::cout << "----------------found G1 solution-----------------------" << std::endl;
    for (auto move : sequence){
        std::cout << faceToStr(move.face) << " " << move.direction << " " << move.times << "\n";
        _cube.applyMove(move);
    }
    return sequence;
}

const std::vector<t_move> Thistlethwaite::getSequence(){
    std::vector<t_move> solveSequence; 

    auto phaseSequence = G0();
    solveSequence.insert(solveSequence.end(), phaseSequence.begin(), phaseSequence.end());

    // phaseSequence = G1();
    // solveSequence.insert(solveSequence.end(), phaseSequence.begin(), phaseSequence.end());

    std::cout << "----------------found Final Sequence-----------------------" << std::endl;

    for (auto move : solveSequence){
        std::cout << faceToStr(move.face) << " " << move.direction << " " << move.times << "\n";
        // _cube.applyMove(move);
    }
    return (solveSequence);
}



bool sameAxis(Face a, Face b) {
    return ((a == U || a == D) && (b == U || b == D)) || ((a == L || a == R) && (b == L || b == R));
}

void Thistlethwaite::IDAStarBaseIteration(std::vector<t_move> &sequence){
    unsigned limit = _currentHeuristic(_cube);
    std::unordered_set<int> visited;

    while (true) {
        Cube workingCube(_cube);
        visited.clear();

        int result = IDAStar(0, limit, workingCube, sequence, NULL_FACE, visited);
        if (result == FOUND)
            break;

        sequence.clear();
        if (result == INT_MAX)
            throw("No possible solutions for this cube.");
        limit = result;

    }
}

int Thistlethwaite::IDAStar(unsigned depth, unsigned limit, Cube &cube, std::vector<t_move> &sequence, Face lastMove, std::unordered_set<int> &visited){
    int key = _cachingCondition(cube);
    if (visited.count(key) > 0)
        return ALREADY_VISITED;

    unsigned h = _currentHeuristic(cube);
    unsigned f = depth + h;
    int newMinLimit = INT_MAX ;

    if (f > limit)
        return f;
    if (h == 0)
        return FOUND;

    visited.insert(key);
    for (auto &move : _allowedMoves){

        if (sameAxis(move.face, lastMove))
            continue;

        cube.applyMove(move);
        sequence.push_back(move);
        int result = IDAStar(depth + 1, limit, cube, sequence, move.face, visited);

        if (result == FOUND)
            return result;

        sequence.pop_back();
        cube.undo(move);

        if (result != ALREADY_VISITED)
            newMinLimit = std::min(newMinLimit, result);
    }

    return newMinLimit;
    
}

Thistlethwaite::~Thistlethwaite(){
}