#pragma once
#include <string>

class PropertiesLoader
{
public:
	void load(std::string propertiesPath);
private:
	enum class PropertyType {
		PawnsPosFact,
		KnightsPosFact,
		BishopsPosFact,
		RocksPosFact,
		QueenPosFact,
		KingsPosFact
	};
	struct Property {
		PropertyType propertyType;
		int blackFactor;
		int whiteFactor;
		Property(PropertyType propType, int black, int white) :propertyType(propType), blackFactor(black), whiteFactor(white) {}
	};
	void apllyProperty(Property);
	Property loadPropertyLine(std::string line);
	PropertyType getPropertyType(std::string propTypeStr);
};

