#include <iostream>
#include "main.hpp"


std::ostream& operator<<(std::ostream& os, const t_move&) {
    return os << "t_move{}";
}

void printParser(const Parser& p, const std::string& label) {
    std::cout << label << " rawMoves: " << p.getRawMoves() << ", moves.size(): " << p.getMoves().size() << "\n";
}

int main() {

    return 0;
}