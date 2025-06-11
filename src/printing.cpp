#include "utils/printing.hpp"

std::string faceToStr(Face face){
    switch (face)
    {
    case U:
        return "U";
    case D:
        return "D";
    case L:
        return "L";
    case R:
        return "R";
    case F:
        return "F";
    case B:
        return "B";
    default:
        return "error";
    }
}


std::string colorToStr(Color color){
    switch (color)
    {
    case WHITE:
        return "W";
    case YELLOW:
        return "Y";
    case GREEN:
        return "G";
    case BLUE:
        return "B";
    case RED:
        return "R";
    case ORANGE:
        return "O";
    default:
        return "error";
    }
}


std::string localToStr(LocalCoordinate local){
    switch (local)
    {
        case TOP:
            return "TOP";
        case DOWN:
            return "DOWN";
        case LEFT:
            return "LEFT";
        case RIGHT:
            return "RIGHT";
        default:
            return "error";
    }
}

void printVector(const std::vector<int> &vect){
    std::cout << "[ ";
    for (auto &component : vect){
        std::cout << component << ", ";
    }
    std::cout <<  " ]"<< std::endl;
}

void printLocalFaces(std::map<Face, std::map<LocalCoordinate, std::vector<int>>> localCoordinates){
    for (auto &[face, locals] : localCoordinates) {
        std::cout << faceToStr(face) << " : " << std::endl;
        for (auto &[coor, vect] : locals) {
            std::cout << localToStr(coor) << " : " ;
            printVector(vect);
        }
    }
}