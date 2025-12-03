#ifndef MY_FILE_MANAGER
#define MY_FILE_MANAGER
#include "debug.h"

static class FileManager {
public:
	FileManager();
	~FileManager();

};

typedef std::shared_ptr<FileManager> FileManagerPtr;
#endif