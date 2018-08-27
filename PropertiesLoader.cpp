#include "PropertiesLoader.h"

#include <fstream>
#include <exception>
#include <assert.h>
#include "Utilities.hpp"
#include "Properties.h"

void PropertiesLoader::load(std::string propertiesPath)
{
	std::fstream propertiesFile;
	propertiesFile.open(propertiesPath, std::ios_base::in);
	std::string propertyLine;
	while (propertiesFile >> propertyLine) {
		Property prop = loadPropertyLine(propertyLine);
		apllyProperty(prop);
	}
}

void PropertiesLoader::apllyProperty(Property prop)
{
	switch (prop.propertyType)
	{
	case PropertyType::PawnsPosFact:
		Properies::pawnsPosFact = { prop.blackFactor, prop.whiteFactor };
		break;
	case PropertyType::KnightsPosFact:
		Properies::knightsPosFact = { prop.blackFactor, prop.whiteFactor };
		break;
	case PropertyType::BishopsPosFact:
		Properies::bishopsPosFact = { prop.blackFactor, prop.whiteFactor };
		break;
	case PropertyType::RocksPosFact:
		Properies::rocksPosFact = { prop.blackFactor, prop.whiteFactor };
		break;
	case PropertyType::QueenPosFact:
		Properies::queenPosFact = { prop.blackFactor, prop.whiteFactor };
		break;
	case PropertyType::KingsPosFact:
		Properies::kingsPosFact = { prop.blackFactor, prop.whiteFactor };
		break;
	default:
		throw std::exception("Unknown property type on apply function");
	}
}

PropertiesLoader::Property PropertiesLoader::loadPropertyLine(std::string line)
{
	auto propertyData = split(line, ';');

	assert(propertyData.size() == 3);
	return Property(getPropertyType(propertyData[0]), std::stoi(propertyData[1]), std::stoi(propertyData[2]));
}

PropertiesLoader::PropertyType PropertiesLoader::getPropertyType(std::string propTypeStr)
{
	switch (getHash(propTypeStr.c_str()))
	{
	case getHash("pawnsPosFact"):
		return PropertyType::PawnsPosFact;
	case getHash("knightsPosFact"):
		return PropertyType::KnightsPosFact;
	case getHash("bishopsPosFact"):
		return PropertyType::BishopsPosFact;
	case getHash("rocksPosFact"):
		return PropertyType::RocksPosFact;
	case getHash("queenPosFact"):
		return PropertyType::QueenPosFact;
	case getHash("kingsPosFact"):
		return PropertyType::KingsPosFact;
	default:
		throw std::exception(std::string("Property type not found for" + propTypeStr).c_str());
	}
}


