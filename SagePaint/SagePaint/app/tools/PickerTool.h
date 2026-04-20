#pragma once
#include <glm/ext/vector_int2.hpp>

#include <nlohmann/json.hpp>
#include <string>

enum PickerMode {
	SINGLELAYER,
	ALL_LAYER
};
class PickerTool {
public:
	static void Pick();
	static PickerMode mode;
	static void ShowUI();
	static std::string ConfigString();
	static void LoadConfig(const nlohmann::json& j);
	static glm::ivec2 downPos;

};