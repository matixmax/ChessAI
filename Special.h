#pragma once
#include <unordered_map>
#include <string>
#include <vector>

typedef std::unordered_map<std::string, std::string> dictionary;
typedef std::unordered_map<std::string, std::vector<std::string>> multiDictionary;

class Special
{
public:
    Special(std::string name = "special");
    virtual ~Special() = default;
    virtual void update() = 0;
    virtual bool isMyCode(std::pair<int, int> code) = 0;
private:
    static std::vector<std::string> m_instances;
};