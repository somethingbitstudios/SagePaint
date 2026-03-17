#include "ToolManager.h"
TOOL_TYPE ToolManager::tool_type = TOOL_PENCIL;
void ToolManager::SetTool(TOOL_TYPE type) {
	tool_type = type;
	//TODO: possibly initiation of tool, like PencilTool::Init()?
}