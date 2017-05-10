#include "Settings.h"
#include <iostream>

using namespace std;

Settings::Settings() :
    Special("settings"), m_showPositions(false), m_settings(dictionary()), m_acceptableVal(multiDictionary())
{
    Settings::addSetting("show_positions", vector<string>({ "true", "false" }));
}

void Settings::addSetting(string name, vector<string> acceptValues)
{
    m_settings[name] = acceptValues[0];
    m_acceptableVal[name] = acceptValues;
}

void Settings::update()
{
    showText();
    string line;
    while (cin >> line) {
        if (line == "exit")
            break;
        else {
            if (m_settings.count(line)) {
                setValue(line);
                break;
            }
            else {
                cout << "bad command" << endl;
            }
        }
    }
    updateAllSettings();
}

void Settings::showText()
{
    const int columnSize = 20;
    cout << "Settings:" << endl;

    cout.width(columnSize);
    cout << left << "show_positions" << "= " << m_showPositions << endl;

    cout << "write some of settings name to set it:" << endl;
}

void Settings::setValue(string name)
{
    string value;
    cout << "set value: ";
    while (cin >> value) {
        if (isAcceptalbleVal(name, value)) {
            m_settings[name] = value;
            break;
        }
        else {
            cout << "incorrect value" << endl;
            cout << "allowed values: " << getAcceptableValues(name) << endl;
            cout << "set value: ";
        }
    }
}

bool Settings::isAcceptalbleVal(string name, string value)
{
    return find(m_acceptableVal[name].begin(), m_acceptableVal[name].end(), value)
        != m_acceptableVal[name].end();
}

string Settings::getAcceptableValues(string name)
{
    string result = "";
    for (string val : m_acceptableVal[name]) {
        result += val + " ";
    }
    return result;
}

void Settings::updateAllSettings()
{
    std::hash<string> getHash;
    for (auto setting : m_settings) {
        size_t name = getHash(setting.first);
        if (name == getHash("show_positions"))
            m_showPositions = (setting.second == "true") ? true : false;
        else
            throw runtime_error("undefined setting with name: " + setting.first);
    }
}

bool Settings::isMyCode(pair<int, int> code)
{
    return code == settingsCode;
}