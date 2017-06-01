#pragma once
#include <sstream>
#include <array>

class Properies {
public:
    static const std::array<int, 2> pawnsPosFact;
    static const std::array<int, 2> knightsPosFact;
    static const std::array<int, 2> bishopsPosFact;
    static const std::array<int, 2> rocksPosFact;
    static const std::array<int, 2> queenPosFact;
    static const std::array<int, 2> kingsPosFact;

    static std::string addToStream();
    static void addToStream(std::stringstream &stream, std::string name, const std::array<int,2>& val);
};