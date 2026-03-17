#pragma once
enum TOOL_TYPE {
	TOOL_PENCIL,
	TOOL_LINE
};
class ToolManager {
public:
	static TOOL_TYPE tool_type;
	static void SetTool(TOOL_TYPE type);
};