#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "structs.hpp"
#include "utils/ParserUtils.hpp"


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
    Parser& operator=(Parser&& toMove) noexcept;
    
    Parser(Parser&& other) noexcept ;
    const std::vector<t_move> getMoves() const;

    const std::string getRawMoves() const;

    ~Parser();
};


