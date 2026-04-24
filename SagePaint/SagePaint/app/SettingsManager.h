#pragma once
#include <vector>
#include <string>
class SettingsManager {
public:
	static void LoadConfig();
	static void LoadToolConfig();
	static void LoadBindingConfig();
	static void SaveConfig();
	static void SaveToolConfig();
	static void SaveBindingConfig();
	static void ShowUI();

	static void SaveProjectPathToRecent();
	static void LoadRecentProjectPaths();
	static bool OpenRecent(int index);
	static bool showWindow;
//private:
	static bool openLatest;
	static std::vector<std::string> recentFiles;
};