#pragma once
enum TOOL_TYPE {
	TOOL_PENCIL,
	TOOL_LINE,
	TOOL_SHAPE,
	TOOL_FILL,
	TOOL_SELECT
};
class ToolManager {
public:
	static TOOL_TYPE tool_type;
	static void SetTool(TOOL_TYPE type);
};  