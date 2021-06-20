#include "FileWriter.h"

void WriteToFile(const std::string& filepath, const std::string& text) {
	std::fstream myfile;
	myfile.open(filepath, std::fstream::out);
	myfile << text;
	myfile.close();
}

void AppendToFile(const std::string& filepath, const std::string& text) {
	std::fstream myfile;
	myfile.open(filepath, std::fstream::out | std::fstream::app);
	myfile << text;
	myfile.close();
}