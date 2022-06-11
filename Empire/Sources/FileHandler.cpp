#include "Headers/FileHandler.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"

FileHandler* FileHandler::Instance()
{
	static FileHandler* s_Instance = new FileHandler();
	return s_Instance;
}

FileHandler::FileHandler()
{
}

FileHandler::~FileHandler()
{
}

char* FileHandler::LoadImage(const char* filepath, int* width, int* height, int* comp)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filepath, width, height, comp, 4);
	return (char*)data;
}

void FileHandler::LoadText(const char* filepath, std::string& returnString)
{
	std::ifstream file(filepath);

	if (file)
		returnString.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	else
		printf("Unable to open file &s\n", filepath);
}
