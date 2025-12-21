#include "FileManager.h"
#include <stb_image.h>

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