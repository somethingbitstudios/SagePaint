#pragma once
class SettingsManager {
public:
	static void LoadConfig();
	static void LoadToolConfig();
	static void SaveConfig();
	static void SaveToolConfig();
	static void ShowUI();
	static bool showWindow;
//private:
	static bool openLatest;
};