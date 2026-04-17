#pragma once
#include <string>
enum PencilMode {
	PENCIL_NORMAL,
	PENCIL_SIMPLE
};
class PencilTool {
public:
	static void Stroke();
	static void StrokeStart();
	static void StrokeEnd();
	static void ShowUI();
	static std::string ConfigString();
private:
	static float strokeSize;
	static PencilMode mode;
}; 