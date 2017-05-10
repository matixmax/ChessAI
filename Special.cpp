
#include "Special.h"

Special::Special(std::string name)
{
    if (std::find(m_instances.begin(), m_instances.end(), name) == m_instances.end()) {
        m_instances.push_back(name);
    }
    else {
        throw std::runtime_error("double specials with name: " + name);
    }
}

bool Special::isMyCode(std::pair<int, int> code)
{
    return false;
}

std::vector<std::string> Special::m_instances = std::vector<std::string>();
