#pragma once
#include <vector>
#include <string>

class StringManipulation {

public:
	static std::string RemoveStartBlank(const std::string& s);
	static std::vector<std::string> SplitString(const std::string& s, const char delimiter);
	static std::string RemoveConsecutiveChar(const std::string& string, char consecutive);
	static std::string FloatToString(const float val);

};