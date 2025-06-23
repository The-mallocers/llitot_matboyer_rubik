#include "main.hpp"

#include <iostream>




int main(int argc, char *argv[]) {

    // (void) argc;
    // (void) argv;

    try {
        if (argc != 2){
            std::cout << "You must provide a sequence." << std::endl;
            return 1;
        }
        Parser parser(argv[1]);
        // Parser parser("F");

        Cube cube(3);
        Solver solver(&cube);
        std::cout << "-" << solver.flippedEdgesHeuristic() << "-" << std::endl;

        cube.applyMoves(parser.getMoves());
        solver.solve();
        std::cout << "- G0 -- " << solver.flippedEdgesHeuristic() << " -" << std::endl;
        std::cout << "- G1 -- " << solver.missplacedEDU() << " -" << std::endl;
    } catch (std::exception err){
        std::cout << " uuuuh ok" << std::endl;

        std::cout << err.what() << " uuuuh ok" << std::endl;
    }




    // for (int i = 0 ; i < 100000; i++){
    //     // Cube cubeCpy(cube);
    //     cube.applyMoves(parser.getMoves());
    //     cube.undo(parser.getMoves()[0]);
    // }
    return 0;
}