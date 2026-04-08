#include "InputFunctions.h"

#include "../CanvasManager.h"
#include "../tools/PencilTool.h"
#include "../tools/ToolManager.h"
#include "../tools/LineTool.h"
#include "../tools/FillTool.h"
#include "../tools/ShapeTool.h"
#include "../tools/SelectTool.h"
#include "../ui/UIManager.h"

void PointerDown() {
    if (UIManager::GetCursorFocus() == FOCUS_CANVAS) {
        switch (ToolManager::tool_type) { //TODO: replace with inline function
        case TOOL_PENCIL:
            PencilTool::StrokeStart();
            break;
        case TOOL_LINE:
            LineTool::LineStart();
            break;
        case TOOL_FILL:
            FillTool::Fill();
            break;
        case TOOL_SHAPE:
            ShapeTool::ShapeStart();
            break;
        case TOOL_SELECT:
            if (SelectTool::dragMode) {

            }
            else {
                SelectTool::SelectStart();
            }
            break;
        }
    }
     
}
void Pointer() {
    if (UIManager::GetCursorFocus() == FOCUS_CANVAS) {
        switch (ToolManager::tool_type) {
        case TOOL_PENCIL:
            PencilTool::Stroke();
            break;
        case TOOL_LINE:
            LineTool::LinePreview();
            break;

        case TOOL_SHAPE:
            ShapeTool::ShapePreview();
            break;
        case TOOL_SELECT:
            if (SelectTool::dragMode) {
                SelectTool::SelectDragRender();
            }
            else {

                SelectTool::SelectPreview();
            }
            break;
        }
    }
}
void PointerUp() {

    if (UIManager::GetCursorFocus() == FOCUS_CANVAS) {
        switch (ToolManager::tool_type) {
        case TOOL_PENCIL:
            PencilTool::StrokeEnd();
            break;
        case TOOL_LINE:
            LineTool::LineEnd();
            break;
        case TOOL_SHAPE:
            ShapeTool::ShapeEnd();
            break;
        case TOOL_SELECT:
            if (SelectTool::dragMode) {
                SelectTool::SelectDragCommit();
            }
            else {
                SelectTool::SelectEnd();
            }
            break;

        }
    }
}

void Drag() {//this is ok until it's dragging something outside of the canvas is needed

    if (UIManager::GetCursorFocus() == FOCUS_CANVAS) {
        CanvasManager::Drag();
    }
}
void ZoomOut() {

    if (UIManager::GetCursorFocus() == FOCUS_CANVAS) {
        CanvasManager::ZoomOut();
    }
}
void ZoomIn() {

    if (UIManager::GetCursorFocus() == FOCUS_CANVAS) {
        CanvasManager::ZoomIn();
    }
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
