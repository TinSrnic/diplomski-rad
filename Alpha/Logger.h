#pragma once
#include "FileWriter.h"

class Logger {

private:

public:
	static void Log(const std::string& t_text) { AppendToFile("Log.txt", t_text); }
	static void Error(const std::string& t_error) { AppendToFile("Err.txt", t_error); }
};