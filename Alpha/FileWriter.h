#pragma once
#include <fstream>

void WriteToFile(const std::string& filepath, const std::string& text);
void AppendToFile(const std::string& filepath, const std::string& text);