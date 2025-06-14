#include "main.hpp"
#include <iostream>




int main(int argc, char *argv[]) {
    if (argc != 2){
        std::cout << "You must provide a sequence." << std::endl;
        return 1;
    }
    Parser parser(argv[1]);

    Cube cube(3);

    cube.applyMoves(parser.getMoves());
    return 0;
}