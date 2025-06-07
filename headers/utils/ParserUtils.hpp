#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <string_view>

#include "enums.hpp"
#include "structs.hpp"


bool tokenIsValid(std::string token);
t_move createMove(std::string token);
std::vector<std::string> tokenizer(std::string &input, char splitter);