#include "CRCChecker.h"

using namespace std;

bool CRCChecker::WriteCRC(HANDLE hFile) {

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	DWORD size = GetFileSize(hFile, NULL);

	DWORD crc = CalcCRC(hFile, size);
	DWORD realBytesWritten;

	WriteFile(hFile, &crc, 2, &realBytesWritten, 0);

	return true;
}

bool CRCChecker::RewriteCRC(HANDLE hFile) {

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	DWORD size = GetFileSize(hFile, NULL);

	DWORD crc = CalcCRC(hFile, size - 2);
	DWORD realBytesWritten;

	WriteFile(hFile, &crc, 2, &realBytesWritten, 0);

	return true;
}

bool CRCChecker::CheckCRC(HANDLE hFile) {

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	LARGE_INTEGER size;
	GetFileSizeEx(hFile, &size);
	DWORD crc = CalcCRC(hFile, size.LowPart - 2);
	DWORD realBytesRead;

	unsigned long infileCRC = 0;

	SetFilePointer(hFile, size.LowPart - 2, NULL, FILE_BEGIN);

	ReadFile(hFile, &infileCRC, 2, &realBytesRead, 0);

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	return crc == infileCRC;
}

unsigned long CRCChecker::CalcCRC(HANDLE hFile, DWORD size) {

	DWORD crc = 0;
	DWORD fileData = 0;
	DWORD realBytesWritten;

	int count = size / 2;
	int remainder = size % 2;

	for (int i = 0; i < count; ++i)
	{
		ReadFile(hFile, &fileData, 2, &realBytesWritten, 0);
		crc = (crc + fileData) % 0x7fff;
	}

	fileData = 0;
	ReadFile(hFile, &fileData, remainder, &realBytesWritten, 0);
	crc = (crc + fileData) % 0x7fff;

	return crc;
}
