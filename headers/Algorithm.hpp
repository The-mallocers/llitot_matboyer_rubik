#pragma once
#include <string>
#include <vector>
#include "enums.hpp"
#include "structs.hpp"

class Algorithm
{
    protected:
        const std::string _name;
        Algorithm(const std::string& name);
    public:
        Algorithm() = delete;
        Algorithm(const Algorithm& toCopy) = delete;
        Algorithm(Algorithm&& toCopy) = delete;
        Algorithm& operator=(const Algorithm& toCopy) = delete;
        Algorithm& operator=(Algorithm&& toMove) noexcept = delete;
        virtual ~Algorithm();

        virtual const std::vector<t_move> getSequence() const = 0;
        const std::string &getName() const;
};

