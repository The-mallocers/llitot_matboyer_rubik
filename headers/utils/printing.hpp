#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "enums.hpp"


void printLocalFaces(std::map<Face, std::map<LocalCoordinate, std::vector<int>>> localCoordinates);
void printVector(const std::vector<int> &vect);
std::string faceToStr(Face face);