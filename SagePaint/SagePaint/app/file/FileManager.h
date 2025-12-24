#pragma once
#include <string>
class FileManager { //might not be a good idea for it to be a class, will make it a namespace in that case
public:
	FileManager();
	~FileManager();
	static unsigned char * LoadImage(std::string filepath,int *width,int * height);
	static unsigned char * LoadImagePlus(std::string filepath,int *width,int * height, int * numOfChannels,int requestedChannelNum);
	static const std::string LoadTextFile(std::string filepath);
	static void Debug();
};