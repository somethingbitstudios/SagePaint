#pragma once
#include <string>

#include <nlohmann/json.hpp>
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
	static void LoadConfig(const nlohmann::json& j);
private:
	static float strokeSize;
	static PencilMode mode;
}; 