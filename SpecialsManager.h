#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include "Special.h"

using SpecialsDictionary = std::unordered_map<std::string, std::unique_ptr<Special>>;

class SpecialsManager
{
public:
    static SpecialsManager& i();

    bool isSpecialCode(std::pair<int, int> code);
    Special* getFormCode(std::pair<int, int> code);
    Special* getFromName(std::string name);

private:
    SpecialsManager();

    SpecialsDictionary m_specials;
};
