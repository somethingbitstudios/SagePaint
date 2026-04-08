#pragma once
enum CursorFocus {
	FOCUS_CANVAS,
	FOCUS_UI

};

class UIManager {
public:
	static void SetCursorFocus(CursorFocus c);
	static CursorFocus GetCursorFocus();
	static bool ShowUI();
private:
	static CursorFocus cursor_focus;
};
//typedef std::shared_ptr<CanvasManager> CanvasManagerPtr;//not needed?