#include "StringManipulation.h"

std::string StringManipulation::RemoveStartBlank(const std::string& s) {
	std::string res = "";

	unsigned int x = 0;
	for (x = 0; x < s.size(); x++) {
		if (s.at(x) != ' ') break;
	}

	res = s.substr(x, s.size());
	return res;
}
std::vector<std::string> StringManipulation::SplitString(const std::string& s, const char delimiter) {
	size_t start = 0;
	size_t end = s.find_first_of(delimiter);

	std::vector<std::string> output;

	while (end <= std::string::npos) {
		output.emplace_back(s.substr(start, end - start));

		if (end == std::string::npos) break;

		start = end + 1;
		end = s.find_first_of(delimiter, start);
	}

	return output;
}
std::string StringManipulation::RemoveConsecutiveChar(const std::string& string, char consecutive) {
	std::string result = std::string();
	bool foundConsecutive = false;

	for each(char c in string) {
		if (c == consecutive && foundConsecutive) continue;
		if (c == consecutive) foundConsecutive = true;
		else foundConsecutive = false;
		result.push_back(c);
	}

	return result;
}
std::string StringManipulation::FloatToString(const float val) {
	std::string result;
	std::string intPart;
	std::string decimalPart;
	int intVal = (int)val;
	int decimal = (int)((val - intVal) * 100);
	bool isNegative = val < 0 ? true : false;

	if (decimal < 0) decimal = -decimal;
	if (intVal < 0) intVal = -intVal;

	if (intVal == 0) intPart += '0';
	while (intVal != 0) {
		intPart += intVal % 10 + '0';
		intVal /= 10;
	}

	std::string temp;
	for (int x = intPart.size() - 1; x >= 0; x--) temp += intPart[x];
	intPart = temp;

	if (decimal < 10) {
		decimalPart += '0';
		decimalPart += decimal + '0';
	} else {
		int lastDecimal = decimal % 10;
		decimal /= 10;
		decimalPart += decimal + '0';
		decimalPart += lastDecimal + '0';
	}

	if (isNegative) result += '-';
	result += intPart + '.' + decimalPart;
	return result;
}