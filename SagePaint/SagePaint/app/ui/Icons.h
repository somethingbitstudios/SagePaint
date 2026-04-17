#include <vector>
#include <imgui.h>
#include <string>

#include <GL/glew.h>
#include "../file/Image.h"
class Icons {
public:
	static std::vector<ImTextureID> icons;
	static void AddIcon(std::string path, std::vector<ImTextureID> &vec);
	
};