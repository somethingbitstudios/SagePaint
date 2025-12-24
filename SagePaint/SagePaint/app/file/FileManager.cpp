#include "FileManager.h"
#include <stb_image.h>
#include <debug.h>
#include <fstream>
#include <sstream>
#include <string>
FileManager::FileManager() {

}
FileManager::~FileManager() {

}

unsigned char * FileManager::LoadImage(std::string filepath, int* width, int* height) {
	int n=0;
	return stbi_load(filepath.c_str(), width, height, &n, 4);
}
unsigned char* FileManager::LoadImagePlus(std::string filepath, int* width, int* height, int* numOfChannels, int requestedChannelNum) {
	return stbi_load(filepath.c_str(), width, height, numOfChannels, requestedChannelNum);
}
void FileManager::Debug() {
	DLOG("it works")
}

const std::string FileManager::LoadTextFile(std::string filepath) {
	std::ifstream file(filepath, std::ios::in);
	if (!file)
		IDLOG("Failed to load file: " << filepath)
	std::ostringstream contents;
	contents << file.rdbuf();
	return contents.str();
}