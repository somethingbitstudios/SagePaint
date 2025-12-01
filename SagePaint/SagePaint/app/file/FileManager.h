#include "debug.h"
#include "stb_image.h"
class FileManager {
public:
	FileManager();
	~FileManager();
};

typedef std::shared_ptr<FileManager> FileManagerPtr;