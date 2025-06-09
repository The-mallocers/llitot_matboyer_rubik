#include "utils/ParserUtils.hpp"

static bool isInSet(char input, const std::string &set) {
    for (unsigned i = 0 ; i < set.size(); ++i){
        if (input == set[i])
            return true;
    }

    return false;
}


static Face getFace(const std::string &face) {
    switch (face[0])
    {
        case 'R':
            return R;
        case 'L':
            return L;
        case 'D':
            return D;
        case 'U':
            return U;
        case 'B':
            return B;
        case 'F':
            return F;
        default:
            return ERROR;
    }
}

static Direction getDirection(const std::string &token){
    if (token.size() > 1 && token[1] == '\'')
        return ANTI_CLOCK_WISE;
    return CLOCK_WISE;
}

static int getTimes(const std::string &token){
    if (token.size() > 1 && token[1] == '2')
        return 2;
    return 1;
}



std::vector<std::string> tokenizer(std::string &input, char splitter) {

    auto tokens = std::views::split(input, splitter) 
        | std::views::transform([](auto &&subrange) {
            return (std::string(&*subrange.begin() , std::ranges::distance(subrange)));
        });

    return {tokens.begin(), tokens.end()};
}


bool tokenIsValid(std::string token) {
    if (token.empty())
        throw std::invalid_argument("Only one space is allowed between two moves in a sequence.");
    if (token.size() > 2)
        throw std::invalid_argument("The move > " + token + " < is not valid. please use the standard notation");
    for (unsigned i = 0 ; i < token.size(); ++i){
        if (i == 0 && !isInSet(token[i], "UDRLBF"))
            return false;
        if (i == 1 && !isInSet(token[i], "'2"))
            return false;
    }

    return true;
}

t_move createMove(std::string token) {

    t_move move;

    move.face = getFace(token);
    move.direction = getDirection(token);
    move.times =  getTimes(token);


    return (move);
}