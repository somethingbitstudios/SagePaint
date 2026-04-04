#pragma once
#include <string>
class ProjectManager {
public:
	static bool dirty;
	static std::string name;
	static bool ShowFileUI();
	static bool Exit();
	static bool Save(std::string path);
	static bool SaveAs();
	static bool Open();
	static bool New();
};