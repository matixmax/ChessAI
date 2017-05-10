#include "SpecialsManager.h"
#include "Settings.h"

using namespace std;

SpecialsManager::SpecialsManager() :m_specials(SpecialsDictionary())
{
	m_specials["settings"] = std::make_unique<Settings>();
}

Special* SpecialsManager::getFormCode(pair<int, int> code)
{
	for (auto& special : m_specials) {
		if (special.second.get()->isMyCode(code))
			return special.second.get();
	}
	throw runtime_error("unknown code of special: " + to_string(code.first) + " " + to_string(code.second));
}

Special* SpecialsManager::getFromName(string name)
{
	return m_specials[name].get();
}

SpecialsManager& SpecialsManager::i()
{
	static SpecialsManager instance;
	return instance;
}

bool SpecialsManager::isSpecialCode(pair<int, int> code)
{
	for (auto& special : m_specials) {
		if (special.second.get()->isMyCode(code))
			return true;
	}
	return false;
}
