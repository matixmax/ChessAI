#pragma once
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> split(std::string input, char delim) {
	std::stringstream inputStream(input);
	std::string segment;
	std::vector<std::string> result;

	while (std::getline(inputStream, segment, delim))
	{
		result.push_back(segment);
	}
	return result;
}

constexpr unsigned int getHash(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (getHash(str, h + 1) * 33) ^ str[h];
}