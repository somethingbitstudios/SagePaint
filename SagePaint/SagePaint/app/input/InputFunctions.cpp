#include "InputFunctions.h"

#include "../CanvasManager.h"
#include "../tools/PencilTool.h"
#include "../tools/ToolManager.h"
#include "../tools/LineTool.h"
#include "../tools/FillTool.h"
#include "../tools/ShapeTool.h"
#include "../tools/SelectTool.h"
#include "../ui/UIManager.h"

bool contains(std::vector<int> &v, int value) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == value)return true;
    }
    return false;
}
void FunctionData::AddBind(int keyid,int mode) {
    if (mode == 0) { //DOWN
        if (!contains(key, keyid)) {
            DLOG("ADDED")
                key.push_back(keyid);
            //add function to key
            KeyFunction a;
            a.func = func;
            a.context=KEY_CONTEXT_DEFAULT;
            a.priority = 0;
            InputManager::keyMap.keyMap[keyid].funcs.emplace_back(a);
        }
    }
    else if (mode == 1) {//HOLD
        if (!contains(keyHold, keyid)) {
            DLOG("ADDED")
                keyHold.push_back(keyid);
            //add function to key
            KeyFunction a;
            a.func = func;

            a.context = KEY_CONTEXT_DEFAULT;
            a.priority = 0;
            InputManager::keyMap.keyMap[keyid].funcsHold.emplace_back(a);
        }
    }
    else { //UP
        if (!contains(keyUp, keyid)) {
            DLOG("ADDED")
                keyUp.push_back(keyid);
            //add function to key
            KeyFunction a;
            a.func = func;

            a.context = KEY_CONTEXT_DEFAULT;
            a.priority = 0;
            InputManager::keyMap.keyMap[keyid].funcsRelease.emplace_back(a);
        }
    }
    
}

void FunctionData::RemoveBind(int keyid, int mode) {
    if (mode == 0) { //DOWN
        if (contains(key, keyid)) {
            //DLOG("REMOVING");

            key.erase(std::remove(key.begin(), key.end(), keyid), key.end());

            auto& funcs = InputManager::keyMap.keyMap[keyid].funcs;
            //this is confusing, meh
            funcs.erase(std::remove_if(funcs.begin(), funcs.end(),
                [this](const KeyFunction& kf) {
                    return kf.func == this->func;
                }),
                funcs.end()
            );
        }
    }
    else if (mode == 1) { //HOLD
        if (contains(keyHold, keyid)) {
            DLOG("REMOVING");

            keyHold.erase(std::remove(keyHold.begin(), keyHold.end(), keyid), keyHold.end());

            auto& funcsHold = InputManager::keyMap.keyMap[keyid].funcsHold;
            funcsHold.erase(std::remove_if(funcsHold.begin(), funcsHold.end(),
                [this](const KeyFunction& kf) {
                    return kf.func == this->func;
                }),
                funcsHold.end()
            );
        }
    }
    else { //UP
        if (contains(keyUp, keyid)) {
            DLOG("REMOVING");

            keyUp.erase(std::remove(keyUp.begin(), keyUp.end(), keyid), keyUp.end());

            auto& funcsRelease = InputManager::keyMap.keyMap[keyid].funcsRelease;
            funcsRelease.erase(std::remove_if(funcsRelease.begin(), funcsRelease.end(),
                [this](const KeyFunction& kf) {
                    return kf.func == this->func;
                }),
                funcsRelease.end()
            );
        }
    }
}


void PointerDown() {
    if (UIManager::GetCursorFocus() == FOCUS_CANVAS || CanvasManager::focusedOnCanvas) {
        CanvasManager::focusedOnCanvas = true;
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
    if (/*UIManager::GetCursorFocus() == FOCUS_CANVAS ||*/ CanvasManager::focusedOnCanvas) {
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

    if (/*UIManager::GetCursorFocus() == FOCUS_CANVAS ||*/ CanvasManager::focusedOnCanvas) {
        CanvasManager::focusedOnCanvas = false;
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
static bool dragging = false;
void Drag() {//this is ok until it's dragging something outside of the canvas is needed

    if (UIManager::GetCursorFocus() == FOCUS_CANVAS || CanvasManager::focusedOnCanvas||dragging) {
        CanvasManager::focusedOnCanvas = true;
        dragging = true;
        CanvasManager::Drag();
    }
}
void DragUp() {//this is ok until it's dragging something outside of the canvas is needed
    if (dragging) {
        CanvasManager::focusedOnCanvas = false;
        dragging = false;
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
    { DragUp, "DragUp", "finishes dragging the canvas",  0 },
    { ZoomOut, "ZoomOut", "zooms the canvas out",  0 },
    { ZoomIn, "ZoomIn", "zooms the canvas in",  0 },

    //not sure if this will be useful, isntead of switching to tool, you can press button to use... might get removed later TODO remove?
    { PencilTool::Stroke, "PencilTool::Stroke", "Executes the pencil stroke regardless of selected tool",  8 },
    //{ LineTool::LineStart, "LineStart", "Begins a line, regarless of chosen tool",  0 }, //TODO: move to a different file than canvasmanager? Like LineTool.cpp?

    //just an example input to show myself that I can call funcs from other files
};
}
