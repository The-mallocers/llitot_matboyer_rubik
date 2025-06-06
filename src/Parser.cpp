#include "Parser.hpp"

Parser::Parser() : _rawMoves(""), _moves(0){
}

Parser::Parser(const Parser& toCopy) : _rawMoves(toCopy._rawMoves), _moves(toCopy._moves){
}

Parser::Parser(const std::string rawMoves) : _rawMoves(rawMoves), _moves(0){
}

Parser& Parser::operator=(const Parser& toCopy){
    if (&toCopy != this){
        this->_rawMoves = toCopy._rawMoves;
        this->_moves = toCopy._moves;
    }

    return *this;
}

Parser& Parser::operator=(const Parser&& toMove) noexcept{
    if (&toMove != this){
        this->_rawMoves = std::move(toMove._rawMoves);
        this->_moves = std::move(toMove._moves);
    }

    return *this;
}

Parser::Parser(Parser&& toMove) noexcept : _rawMoves(std::move(toMove._rawMoves)), _moves(toMove._moves){
}


const std::vector<t_move> Parser::getMoves() const {
    return _moves;
};

const std::string Parser::getRawMoves() const {
    return _rawMoves;
};



Parser::~Parser(){
}