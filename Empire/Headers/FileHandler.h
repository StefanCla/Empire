#pragma once
#include "Headers/Precomp.h"
#include <fstream>

class FileHandler
{
public:
	FileHandler(const FileHandler& fh) = delete;
	FileHandler& operator=(const FileHandler& fh) = delete;

	static FileHandler* Instance();

	char* LoadImage(const char* filepath, int* width, int* height, int* comp);
	void LoadText(const char* filepath, std::string& returnString);

private:
	FileHandler();
	~FileHandler();

};
