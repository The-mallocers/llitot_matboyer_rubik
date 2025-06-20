#pragma once

#include <vector>
#include <stdexcept>

#include <iostream>


int doProduct(const std::vector<int>&A, const std::vector<int> &B);
std::vector<int> crossProduct3(const std::vector<int>&A, const std::vector<int> &B);
std::vector<int> multiplyVector(const std::vector<int>&A, int multiplier);
std::vector<int> addIntToVector(const std::vector<int>&A, int toAdd);
std::vector<int> subVectors(const std::vector<int>&A, const std::vector<int> &B);