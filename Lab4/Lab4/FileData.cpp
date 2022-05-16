#include "FileData.h"



FileData::FileData(HANDLE hFile) {

	DWORD real;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	ReadFile(hFile, &msgCount, 4, &real, NULL);
	ReadFile(hFile, &mailSize, 4, &real, NULL);
	ReadFile(hFile, &maxSize, 4, &real, NULL);

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
}