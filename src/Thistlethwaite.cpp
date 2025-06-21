#include "Thistlethwaite.hpp"

Thistlethwaite::Thistlethwaite(Solver&solver, Cube &cube) : Algorithm("Thistlethwaite", solver, cube){
}

const std::vector<t_move>Thistlethwaite::G0 (){
    Parser parser("F B R L U D F' B' R' L' U' D' F2 B2 R2 L2 U2 D2");
    // parser.parse;
    _allowedMoves = parser.getMoves();
    _currentHeuristic = [&](Cube &cube) { return _solver.flippedEdgesHeuristic(cube);};
    unsigned limit = _currentHeuristic(_cube);
    std::vector<t_move> sequence;
    int result;
    while (true) {
        std::cout << "---- new iter start ----" << std::endl;
        Cube workingCube(_cube);  // always reset to original state
        result = IDAStar(0, limit, workingCube, sequence, NULL_FACE);
        if (result == FOUND) 
            break;

        sequence.clear();
        if (result == INT_MAX)
            throw("No possible solutions for this cube.");
        limit = result;
        std::cout << "---- iter end ----" << std::endl;

    }

    std::cout << "----------------found G0 solution-----------------------" << std::endl;
    for (auto move : sequence){
        std::cout << faceToStr(move.face) << " " << move.direction << " " << move.times << "\n";
    }
    return sequence;
}

const std::vector<t_move> Thistlethwaite::getSequence(){
    std::vector<t_move> test; 

    G0();
    return (test);
}



bool sameAxis(Face a, Face b) {
    return ((a == U || a == D) && (b == U || b == D)) ||
           ((a == F || a == B) && (b == F || b == B) )||
           ((a == L || a == R) && (b == L || b == R));
}

int Thistlethwaite::IDAStar(unsigned depth, unsigned limit, Cube &cube, std::vector<t_move> &sequence, Face lastMove){
    unsigned h = _currentHeuristic(cube);

    unsigned f = depth + h;
    int newMinLimit = INT_MAX ;

    if (f > limit)
        return f;
    if (h == 0)
        return FOUND;
    for (auto &move : _allowedMoves){
        // std::cout << faceToStr(lastMove) << std::endl;
        if (sameAxis(move.face, lastMove)){
            continue;
        }

        Cube newCube(cube);
        newCube.applyMove(move);
        sequence.push_back(move);
        
        int result = IDAStar(depth + 1, limit, newCube, sequence, move.face);

        if (result == FOUND){
            return result;
        }

        sequence.pop_back();
        // std::cout << "result biittchhh "  << result << std::endl;
        newMinLimit = std::min(newMinLimit, result);
        // std::cout << newMinLimit << std::endl;

    }

    return newMinLimit;
    
}

Thistlethwaite::~Thistlethwaite(){
}