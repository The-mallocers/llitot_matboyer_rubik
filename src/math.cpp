#include "math.hpp"

int doProduct(const std::vector<int>&A, const std::vector<int> &B){
    if (A.size() != B.size()){
        throw std::invalid_argument("Both vectors must be of the same size.");
        return INT_MIN;
    }

    int product = 0;
    for (unsigned i = 0; i < A.size(); i++){
        product += A[i] * B[i];
    }

    return product;
}


std::vector<int> crossProduct3(const std::vector<int>&A, const std::vector<int> &B){
    if (A.size() != 3 || B.size() != 3)
        throw std::invalid_argument("Both vectors must be of size 3");

    std::vector<int> C;
    C.resize(3);

    C[0] = A[1]*B[2] - A[2] * B[1];
    C[1] = A[2]*B[0] - A[0] * B[2];
    C[2] = A[0]*B[1] - A[1] * B[0];

    return C;
}

std::vector<int> multiplyVector(const std::vector<int>&A, int multiplier){
    std::vector<int> result;
    result.resize(A.size());
    for (unsigned i = 0 ; i < A.size() ; i++){
        result[i] = A[i] * multiplier;
    }

    return result;
}

std::vector<int> addIntToVector(const std::vector<int>&A, int toAdd) {
    std::vector<int> result;
    result.resize(A.size());
    for (unsigned i = 0 ; i < A.size() ; i++){
        result[i] = A[i] + toAdd;
    }

    return result;
}

std::vector<int> subVectors(const std::vector<int>&A, const std::vector<int> &B){
    if (A.size() != B.size()){
        throw std::invalid_argument("Both vectors must be of the same size.");
        // return INT_MIN;
    }
    std::vector<int> result;
    result.resize(A.size());
    for (unsigned i = 0 ; i < A.size() ; i++){
        result[i] = A[i] - B[i];
    }

    return result;
}