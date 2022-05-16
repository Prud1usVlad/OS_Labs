#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <fileapi.h>

using namespace std;

#ifndef FileData
#define FuleData
class FileData
{
public:

	unsigned long msgCount;
	unsigned long mailSize;
	unsigned long maxSize;

	FileData(HANDLE hFile);
};
#endif

