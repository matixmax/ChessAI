#pragma once
#include <utility>
#include "Special.h"

class Settings : public Special
{
public:
	explicit Settings();
	void update() override;
	bool isMyCode(std::pair<int, int> code) override;

private:
	void showText();
	void updateAllSettings();
	void setValue(std::string name);
	void addSetting(std::string name, std::vector<std::string> acceptValues);
	bool isAcceptalbleVal(std::string name, std::string value);
	std::string getAcceptableValues(std::string name);

	const std::pair<int, int>settingsCode = { -1,-1 };
	dictionary m_settings;
	multiDictionary m_acceptableVal;
	bool m_showPositions;
};

