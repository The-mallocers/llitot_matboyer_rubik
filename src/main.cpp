#include "main.hpp"
#include <iostream>



static void listCube(Cube &cube){
    for (auto &i : cube.getData()){
        std::cout << i << std::endl;
    }

    std::cout << ">------------<" << std::endl;

}

int main() {
    Cube cube(3);

    listCube(cube);

    Cube ebuc = std::move(cube);
    listCube(cube);
    listCube(ebuc);
    
    return 0;
}