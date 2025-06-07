#include "Parser.hpp"

Parser::Parser() : _rawMoves(""), _moves(0){}
Parser::Parser(const Parser& toCopy) : _rawMoves(toCopy._rawMoves), _moves(toCopy._moves){}
Parser::Parser(const std::string rawMoves) : _rawMoves(rawMoves), _moves(0){
    try {
        this->parse();
    } catch (const std::invalid_argument &err) {
        std::cerr << err.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

Parser& Parser::operator=(const Parser& toCopy){
    if (&toCopy != this){
        this->_rawMoves = toCopy._rawMoves;
        this->_moves = toCopy._moves;
    }

    return *this;
}

Parser& Parser::operator=(Parser&& toMove) noexcept{
    if (&toMove != this){
        this->_rawMoves = std::move(toMove._rawMoves);
        this->_moves = std::move(toMove._moves);
    }

    return *this;
}

Parser::Parser(Parser&& toMove) noexcept : _rawMoves(std::move(toMove._rawMoves)), _moves(std::move(toMove._moves)){
}

void Parser::parse() {

    if (_rawMoves.empty())
        throw std::invalid_argument("The scrambling sequence cannot be empty.");

    std::vector<std::string> tokens = tokenizer(_rawMoves, ' ');
    for (auto &token : tokens) {
        if (!tokenIsValid(token))
            throw std::invalid_argument("The move > " + token + " < is not valid. please use the standard notation");
        _moves.push_back(createMove(token));
    }

}


const std::vector<t_move> Parser::getMoves() const {
    return _moves;
};

const std::string Parser::getRawMoves() const {
    return _rawMoves;
};



Parser::~Parser(){
}