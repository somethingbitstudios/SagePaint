#pragma once
#include <string>
class ProjectManager {
public:
	static void Init();
	static bool dirty;
	static std::string name;
	static std::string description;
	static std::string fullPath;
	static bool projectDataDirty;
	static bool ShowFileUI();
	static bool Exit();
	static bool ExitDlgOpen();
	static bool Save(std::string path);
	static bool Open(std::string path);
	static bool New();
	static bool Clear();
	static void Dirty();
};