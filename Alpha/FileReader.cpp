#pragma once
#include "FileReader.h"

std::string ReadFile(std::string filepath) {
	FILE* file;
	fopen_s(&file, filepath.c_str(), "r");
	char c = 0;
	std::string fileSource = "";

	do {
		c = fgetc(file);
		if (c == EOF) break;
		fileSource += c;
	} while (c != EOF);

	fclose(file);
	return fileSource;
}