#include "Thistlethwaite.hpp"

const std::pair<int, int> FOUND = {-1, -1};
const std::pair<int, int> ALREADY_VISITED = {-2, -2};

Thistlethwaite::Thistlethwaite(Solver&solver, Cube &cube) : Algorithm("Thistlethwaite", solver, cube){
    totalIDAStarCalls = 0;
}


bool Thistlethwaite::breaksPreviousPhases(Cube& cube) {    
    unsigned total = 0;
    for (auto &heuristic : _previousHeuristics){

        total += heuristic(cube);
    }
    return total > 0 ? true : false;
}


const std::vector<t_move>Thistlethwaite::G0(){
    Parser parser("L R F B U D L2 R2 F2 B2 U2 D2 L' R' F' B' U' D'");

    std::cout << "- G0 BEGIIIINNNNN -- " << " -" << std::endl;

    // std::cout << "- G0 -- " << _solver.flippedEdgesHeuristic() << " -" << std::endl;
    // std::cout << "- G1 -- " << (_solver.twistedCorners() + _solver.missplaced_DU_MIDDLE())   << " -" << std::endl;
    // std::cout << "- G2 -- " << << " -" << std::endl;
    _allowedMoves = parser.getMoves();
    _currentHeuristic = [&](Cube &cube) { return std::make_pair(_solver.flippedEdgesHeuristic(cube), 0);};
    _cachingCondition = [&](Cube &cube) { return std::make_pair(cube.encodeEdgeOrientation(), 0);};


    std::vector<t_move> sequence;
    IDAStarBaseIteration(sequence);

    std::cout << "----------------found G0 solution-----------------------" << std::endl;
    for (auto move : sequence){
        std::cout << faceToStr(move.face) << " " << move.direction << " " << move.times << "\n";
        _cube.applyMove(move);
    }

    // _previousHeuristics.push_back(_currentHeuristic);
    // std::cout << "nan mais oh :::::: " <<  _currentHeuristic(_cube) << std::endl;

    return sequence;
}


const std::vector<t_move>Thistlethwaite::G1(){

    // Parser parser("F L U R D B F2 B2 U2 D2");
    // Parser parser("L L' R R' F F' B B' U2 D2");
    Parser parser("R L F2 B2 U D");

    _allowedMoves = parser.getMoves();
    //  _currentHeuristic = [&](Cube &cube) { return std::max(_solver.twistedCorners(cube), _solver.missplaced_M_SLICE(cube));}; 
     _currentHeuristic = [&](Cube &cube) { return std::make_pair(_solver.twistedCorners(cube) , _solver.missplaced_M_SLICE(cube));}; 
    _cachingCondition =  [&](Cube &cube) {
    // unsigned long cornerKey = ;
    // unsigned long edgeKey = ;
    // // unsigned long combinedKey = cornerKey * 16 + edgeKey;
    // unsigned long combinedKey = (cornerKey << 6) | edgeKey;
    

    return std::make_pair(cube.encodetwistedCorners(), cube.encodeMissplacedEdges_SLICE_M());
    };

    std::cout << "- G1 BEGIIIINNNNN -- " << " -" << std::endl;

    std::vector<t_move> sequence;
    IDAStarBaseIteration(sequence);

    std::cout << "----------------found G1 solution-----------------------" << std::endl;

    for (auto& meow : _cube.twistTracker)
        std::cout << meow << " " ;

    std::cout <<std::endl;

    for (auto move : sequence){
        std::cout << faceToStr(move.face) << " " << move.direction << " " << move.times << "\n";
        _cube.applyMove(move);
    }

    for (auto& meow : _cube.twistTracker)
        std::cout << meow << " " ;

        std::cout <<std::endl;
    // std::cout << "nan mais oh :::::: " <<  _currentHeuristic(_cube) << std::endl;
    // _previousHeuristics.push_back(_currentHeuristic);




    return sequence;
}

const std::vector<t_move> Thistlethwaite::getSequence(){
    std::vector<t_move> solveSequence;

    auto phaseSequence = G0();
    solveSequence.insert(solveSequence.end(), phaseSequence.begin(), phaseSequence.end());

    phaseSequence = G1();
    solveSequence.insert(solveSequence.end(), phaseSequence.begin(), phaseSequence.end());

    // phaseSequence = G2();
    // solveSequence.insert(solveSequence.end(), phaseSequence.begin(), phaseSequence.end());

    std::cout << "----------------found Final Sequence-----------------------" << std::endl;

    for (auto move : solveSequence){
        std::cout << faceToStr(move.face);
        if (move.direction == ANTI_CLOCK_WISE)
            std::cout << "'" ;
        if (move.times == 2)
            std::cout << move.times;
        std::cout << " ";
        // _cube.applyMove(move);
    }
        std::cout << "\n";

    std::cout << "total calls: " << totalIDAStarCalls << std::endl;

    return (solveSequence);
}



bool sameAxis(Face a, Face b) {
    return ((a == U || a == D) && (b == U || b == D)) || ((a == L || a == R) && (b == L || b == R)) || ((a == F || a == B) && (b == F || b == B));
}

void Thistlethwaite::IDAStarBaseIteration(std::vector<t_move> &sequence){
    std::pair<int, int> limit = _currentHeuristic(_cube);
    
    while (true) {
        std::set<std::pair<unsigned long, unsigned long>> visited;
        Cube workingCube(_cube);

        std::pair<int, int> result = IDAStar(0, limit, workingCube, sequence, NULL_FACE, visited);

        if (result == FOUND)
            break;

            
        sequence.clear();

        if (result.first == INT_MAX && result.second == INT_MAX){
            std::cout << "bruh" << std::endl;
            // throw (std::runtime_error("Cube might be unsolvable"));
            return;
        }
            // throw("No possible solutions for this cube.");
            std::cout << result.first << " " << result.second << " \n";
        limit = result;


    }

}

std::pair<int,int> Thistlethwaite::IDAStar(unsigned depth, std::pair<int, int> &limit, Cube &cube, std::vector<t_move> &sequence, Face lastMove,  std::set<std::pair<unsigned long, unsigned long>> &visited){
    // (void) lastMove;
    std::pair<unsigned long, unsigned long> key = _cachingCondition(cube);
    totalIDAStarCalls++;
    // std::cout << key << " \n";
    if (visited.count(key) > 0)
        return ALREADY_VISITED;

    std::pair<int, int> h = _currentHeuristic(cube);
    // std::cout << h << " \n" ;

    std::pair<int, int> f = {depth + h.first, h.second};
    std::pair<int, int> newMinLimit = {INT_MAX, INT_MAX} ;

    if (f > limit)
        return f;
    if (h.first == 0 && h.second == 0){

        for (auto& meow : cube.twistTracker)
            std::cout << meow << " " ;

        std::cout <<std::endl;

        return FOUND;
    }

    visited.insert(key);
    for (auto &move : _allowedMoves){


        if ( move.face == lastMove) continue;
        if (sameAxis(move.face, lastMove)) {
            // if ((move.face == L && lastMove == R) || (move.face == D && lastMove == U) || (move.face == B && lastMove == F) || move.face == lastMove)
            //     continue;;
            continue;
        }

        cube.applyMove(move);
        sequence.push_back(move);

        // if (breaksPreviousPhases(cube)) {
        //     sequence.pop_back();
        //     cube.undo(move);
        //     continue;
        // }

        std::pair<int,int> result = IDAStar(depth + 1, limit, cube, sequence, move.face, visited);
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