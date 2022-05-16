#pragma once

#include <Windows.h>
#include <filesystem>
#include <fileapi.h>
#include <iostream>

using namespace std;

#ifndef CRC
#define CRC

class CRCChecker
{
public: 

	bool CheckCRC(HANDLE hFile);
	bool WriteCRC(HANDLE hFile);
	bool RewriteCRC(HANDLE hFile);

private: 

	unsigned long CalcCRC(HANDLE hFile, DWORD size);
};

#endif

