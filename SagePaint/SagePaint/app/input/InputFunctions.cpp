#include "InputFunctions.h"

#include "../CanvasManager.h"

void Click() {
    DLOG("Click")
}
void ClickUI() {
    DLOG("Click in UI")
}

std::vector<FunctionData> inputFunctions;

void InitInputFunctions() {
    inputFunctions = {
    //{ Click,   "Click",   "does click",        1 },
    { CanvasManager::Draw, "Draw", "does click in UI",  0 },
    { CanvasManager::Drag, "Drag", "does dragging the canvas",  0 },
    { CanvasManager::ZoomOut, "ZoomOut", "zooms the canvas out",  0 },
    { CanvasManager::ZoomIn, "ZoomIn", "zooms the canvas in",  0 },

    //just an example input to show myself that I can call funcs from other files
};
}