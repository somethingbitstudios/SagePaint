#include "InputFunctions.h"

#include "../file/FileManager.h"

void Click() {
    DLOG("Click")
}
void ClickUI() {
    DLOG("Click in UI")
}

std::vector<FunctionData> inputFunctions;

void InitInputFunctions() {
    inputFunctions = {
    { Click,   "Click",   "does click",        1 },
    { FileManager::Debug, "ClickUI", "does click in UI",  0 },//just an example input to show myself that I can call funcs from other files
};
}