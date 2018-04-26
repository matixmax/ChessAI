#pragma once
#include <sstream>
#include <array>

class Properies {
public:
    static std::array<int, 2> pawnsPosFact;
    static std::array<int, 2> knightsPosFact;
    static std::array<int, 2> bishopsPosFact;
    static std::array<int, 2> rocksPosFact;
    static std::array<int, 2> queenPosFact;
    static std::array<int, 2> kingsPosFact;

    static std::string getFactorsAsString();
    static void addToStream(std::stringstream &stream, std::string name, const std::array<int,2>& val);
};