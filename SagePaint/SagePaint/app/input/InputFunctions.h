#pragma once
#include <functional>
#include <string>
#include <debug.h>

struct FunctionData {
    void (*func)();
	std::string name;
	std::string description;
	int color;//for ui purposes
};

extern std::vector<FunctionData> inputFunctions;
void InitInputFunctions();