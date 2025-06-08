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
