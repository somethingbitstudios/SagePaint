#include "InputFunctions.h"

#include "../CanvasManager.h"
#include "../tools/PencilTool.h"

void PointerDown() {

}
void Pointer() {
    PencilTool::Stroke();//replace with switch depending on tool, then call tool or canvas specific function

}
void PointerUp() {
}

void Drag() {//this is ok until it's dragging something outside of the canvas is needed
    CanvasManager::Drag();
}
void ZoomOut() {
    CanvasManager::ZoomOut();
}
void ZoomIn() {
    CanvasManager::ZoomIn();
}

std::vector<FunctionData> inputFunctions;

void InitInputFunctions() {
    inputFunctions = {
    //{ Click,   "Click",   "does click",        1 },
    { PointerDown, "PointerDown", "Generic cursor action (called on the pressed frame), depends on currently chosen tool / context",  0 },
    { Pointer, "Pointer", "Generic cursor action (called on every held frame), depends on currently chosen tool / context",  0 },
    { PointerUp, "PointerUp", "Generic cursor action (called on the released frame), depends on currently chosen tool / context",  0 },
    { Drag, "Drag", "does dragging the canvas",  0 },
    { ZoomOut, "ZoomOut", "zooms the canvas out",  0 },
    { ZoomIn, "ZoomIn", "zooms the canvas in",  0 },
    //not sure if this will be useful, isntead of switching to tool, you can press button to use... might get removed later TODO remove?
    { PencilTool::Stroke, "PencilTool::Stroke", "Executes the pencil stroke regardless of selected tool",  8 },
    //{ LineTool::LineStart, "LineStart", "Begins a line, regarless of chosen tool",  0 }, //TODO: move to a different file than canvasmanager? Like LineTool.cpp?

    //just an example input to show myself that I can call funcs from other files
};
}
