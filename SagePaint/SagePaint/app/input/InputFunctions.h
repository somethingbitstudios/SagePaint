#pragma once
#include <functional>
#include <string>
#include <debug.h>

struct FunctionData {
    void (*func)();
	std::string name;
	std::string description;
	int color;//for ui purposes
	std::vector<int> key;
	std::vector<int> keyHold;
	std::vector<int> keyUp;
};

extern std::vector<FunctionData> inputFunctions;
void InitInputFunctions();