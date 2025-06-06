#pragma once
#include <string>
#include <vector>
#include "structs.hpp"


class Parser
{
private:
    std::string _rawMoves;
    std::vector<t_move> _moves;
    void parse();

public:
    Parser();
    Parser(const Parser& toCopy);
    Parser(const std::string rawMoves);
    
    Parser& operator=(const Parser& toCopy);
    Parser& operator=(const Parser&& toMove) noexcept;
    
    Parser(Parser&& other) noexcept ;
    const std::vector<t_move> getMoves() const;

    const std::string getRawMoves() const;

    ~Parser();
    // later i'll put custom exceptions here.
};


