#include "MailSystem.h"
#include "FileData.h"
#include <string>
#include <filesystem>
#include <fileapi.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// Basic constructor 
MailSystem::MailSystem(const char* directoryPath) {

	setlocale(LC_ALL, "Ukrainian");
	if (directoryPath == 0) {
		CreateDirectoryA("MailBox", 0);
		loadedCorrectly = true;
		dir = ".\\Mailbox";
	}
	else {
		struct stat buffer;

		if (stat(directoryPath, &buffer) != 0) {
			wcout << L"There is no such directory: " << directoryPath << endl;
			loadedCorrectly = false;
		}
		else
		{
			loadedCorrectly = true;
			dir = directoryPath;
		}
	}

	GetMailSystemInfo();
	
	crcChecker = new CRCChecker();
}

// Creates in directory file name.mail for storing data
bool MailSystem::CreateMailbox(const char* name, unsigned int size) {

	if (!loadedCorrectly) {
		cout << "Object loaded incorrectly. Solve problems and try again" << endl;
		return false;
	}

	string path = dir + "\\" + name + ".mail";

	HANDLE h = FindFirstFileA(path.c_str(), &lastFound);

	if (h != INVALID_HANDLE_VALUE) {
		wcout << L"There already exists a file with name: " << path.c_str() << endl;
		FindClose(h);

		return false;
	}
	else
	{
		FindClose(h);

		h = CreateFileA(
			path.c_str(),
			FILE_GENERIC_READ | FILE_GENERIC_WRITE,
			FILE_SHARE_READ,
			0,
			CREATE_NEW,
			0,
			0
		);

		DWORD writen; 

		// Mails number
		DWORD data = 0;
		WriteFile(h, &data, 4, &writen, 0);

		// Not free space in mailbox
		WriteFile(h, &data, 4, &writen, 0);

		// Mailbox size
		WriteFile(h, &size, 4, &writen, 0);

		data = size % 0x7fff;
		WriteFile(h, &data, 2, &writen, 0);
		CloseHandle(h);

		cout << "Mailbox created" << endl;
	}

}

// Returns mailbox count, and prints detailed info about mailboxes
unsigned long MailSystem::GetMailSystemInfo() {
	string mask = dir + "\\*.mail";

	HANDLE h = FindFirstFileA(mask.c_str(), &lastFound);

	mailboxCount = 0;

	cout << "Files in the directory:" << endl;

	do {

		if (h == INVALID_HANDLE_VALUE) {
			cout << "    --- Empty ---" << endl;
		}
		else {
			
			string path = dir + "\\" + lastFound.cFileName;

			HANDLE fileHandle = CreateFileA(
				path.c_str(),
				FILE_GENERIC_READ | FILE_GENERIC_WRITE,
				FILE_SHARE_READ,
				0,
				OPEN_EXISTING,
				0,
				0);

			FileData * fd = new FileData(fileHandle);

			if (fd->msgCount == 3452816845) {
				cout << "An error occured. It seems that this file is opened in some editor, close it and try again" << endl;
				loadedCorrectly = false;
				FindClose(h);
				CloseHandle(fileHandle);
				return 0xffffffff;
			}

			CloseHandle(fileHandle);

			cout << "    " << ++mailboxCount << " | " << lastFound.cFileName << " | Messages "
				<< fd->msgCount << ", Messages size " << fd->mailSize << ", Mailbox size " << fd->maxSize << endl;

		}
	} while (FindNextFileA(h, &lastFound));

	cout << "Total number of Mailboxes: " << mailboxCount << endl;
	FindClose(h);

	return mailboxCount;
}

// Calls functions to check if file has not been changed
// If file has been changed throws warning
bool MailSystem::CheckFileCRC(HANDLE hFile) {

	if (!loadedCorrectly) {
		cout << "Object loaded incorrectly. Solve problems and try again" << endl;
		return false;
	}


	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Error while opening file" << endl;
		return false;
	}
	else if (crcChecker->CheckCRC(hFile)) {
		return true;
	}
	else {
		int resID = MessageBoxA(
			NULL,
			"Be careful! File of this mailbox has been changed without system's permission. Unexpected result of the special CRC-test. \n Do you want to continue?",
			"Warning",
			MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2
		);

		return resID == IDYES;
	}

}

// Opens file and calls CheckFileCRC function 
HANDLE MailSystem::OpenFile(const char* name) {
	string path = dir + "\\" + name + ".mail";

	HANDLE h = CreateFileA(
		path.c_str(),
		FILE_GENERIC_READ | FILE_GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		0,
		0
	);

	if (!CheckFileCRC(h)) return INVALID_HANDLE_VALUE;

	return h;
}

// Adds message to the mailbox with specified name
bool MailSystem::AddMessage(const char* name, const char* message) {

	if (!loadedCorrectly) {
		cout << "Object loaded incorrectly. Solve problems and try again" << endl;
		return false;
	}

	string path = dir + "\\" + name + ".mail";

	HANDLE hFile = OpenFile(name);
	DWORD size = GetFileSize(hFile, NULL);
	unsigned long msgBytes = strlen(message);

	// Creating file mapping
	HANDLE hMapFile = CreateFileMappingA(hFile, 0, PAGE_READWRITE, 0, size + msgBytes + 2, 0);

	if (hMapFile == NULL) {
		cout << "CreateFileMapping error: " << GetLastError() << endl;
		return false;
	}

	// Getting pointer to mapped file
	LPVOID mappedAddres = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (*((DWORD*)mappedAddres + 1) + msgBytes + 4 > *((DWORD*)mappedAddres + 2)) {
		cout << "Mailbox: " << name << " is out of memory. Can't add last message";
		UnmapViewOfFile(mappedAddres);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		return false;
	}

	*((DWORD*)mappedAddres) += 1;
	*((DWORD*)mappedAddres + 1) += msgBytes;

	byte* pMsgPlace = (byte*)mappedAddres + size - 2;

	memcpy(pMsgPlace, &msgBytes, 4);
	memcpy(pMsgPlace + 4, message, msgBytes);

	UnmapViewOfFile(mappedAddres);
	CloseHandle(hMapFile);

	crcChecker->WriteCRC(hFile);

	CloseHandle(hFile);
	cout << "Message added" << endl;
	return true;
}

// Reads message from the mailbox with specified name
// index starting from 1
string MailSystem::ReadMessage(const char* name, unsigned long index) {

	if (!loadedCorrectly) {
		cout << "Object loaded incorrectly. Solve problems and try again" << endl;
		return "error";
	}

	HANDLE hFile = OpenFile(name);
	DWORD size = GetFileSize(hFile, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return "error";
	}

	// Creating file mapping
	HANDLE hMapFile = CreateFileMappingA(hFile, 0, PAGE_READWRITE, 0, size, 0);

	if (hMapFile == NULL) {
		cout << "CreateFileMapping error: " << GetLastError() << endl;
		return "error";
	}

	// Getting pointer to mapped file
	LPVOID mappedAddres = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (index > *(DWORD *)mappedAddres || index <= 0) {
		cout << "Invalid index" << endl;
		UnmapViewOfFile(mappedAddres);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		return "error";
	}

	DWORD shift = 12;

	for (int i = 1; i < index; ++i) {
		shift += *((DWORD*)((byte*)mappedAddres + shift)) + 4;
	}

	DWORD msgSize = *((DWORD*)((byte*)mappedAddres + shift));
	shift += 4;

	char* data = new char[msgSize + 1];
	memcpy(data, (char *)mappedAddres + shift, msgSize);
	data[msgSize] = '\0';


	UnmapViewOfFile(mappedAddres);
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	string res = data;
	delete[] data;
	return res;
}

// Reads than deletes message from mailbox with specified name
// index starting from 1
string MailSystem::ReadAndDelete(const char* name, unsigned long index) {
	
	string res = ReadMessage(name, index);
	if (res != "error") { 
		Delete(name, index); 
	}
	return res;
}

// Deletes message from mailbox with specified name
// index starting from 1
bool MailSystem::Delete(const char* name, unsigned long index) {

	if (!loadedCorrectly) {
		cout << "Object loaded incorrectly. Solve problems and try again" << endl;
		return false;
	}

	HANDLE hFile = OpenFile(name);
	DWORD size = GetFileSize(hFile, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return false;
	}

	// Creating file mapping
	HANDLE hMapFile = CreateFileMappingA(hFile, 0, PAGE_READWRITE, 0, size, 0);

	if (hMapFile == NULL) {
		cout << "CreateFileMapping error: " << GetLastError() << endl;
		return false;
	}

	// Getting pointer to mapped file
	LPVOID mappedAddres = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (index > *(DWORD*)mappedAddres || index <= 0) {
		cout << "Invalid index" << endl;
		UnmapViewOfFile(mappedAddres);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		return false;
	}

	DWORD shift = 12;
	DWORD toDeleteBytes = 0;
	DWORD toMoveBytes = 0;

	for (int i = 1; i < index; ++i) {
		shift += *((DWORD*)((byte*)mappedAddres + shift)) + 4;
	}

	toDeleteBytes = *((DWORD*)((byte*)mappedAddres + shift)) + 4;
	toMoveBytes = size - shift - toDeleteBytes;

	// If deleting not last message
	if (index < *(DWORD*)mappedAddres) {
		memcpy((char*)mappedAddres + shift, (char*)mappedAddres + shift + toDeleteBytes, toMoveBytes);
	}
	
	*((DWORD*)mappedAddres) -= 1;
	*((DWORD*)mappedAddres + 1) -= toDeleteBytes;

	UnmapViewOfFile(mappedAddres);
	CloseHandle(hMapFile);

	SetFilePointer(hFile, shift + toMoveBytes - 2, 0, FILE_BEGIN);
	SetEndOfFile(hFile);

	crcChecker->WriteCRC(hFile);
	CloseHandle(hFile);
	cout << "Message deleted" << endl;
	return true;
}

// Deletes all mesages from file
bool MailSystem::Clear(const char* name) {
	if (!loadedCorrectly) {
		cout << "Object loaded incorrectly. Solve problems and try again" << endl;
		return false;
	}

	HANDLE hFile = OpenFile(name);
	DWORD size = GetFileSize(hFile, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return false;
	}

	// Creating file mapping
	HANDLE hMapFile = CreateFileMappingA(hFile, 0, PAGE_READWRITE, 0, size, 0);

	if (hMapFile == NULL) {
		cout << "CreateFileMapping error: " << GetLastError() << endl;
		return false;
	}

	// Getting pointer to mapped file
	LPVOID mappedAddres = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);


	*((DWORD*)mappedAddres) = 0;
	*((DWORD*)mappedAddres + 1) = 0;

	UnmapViewOfFile(mappedAddres);
	CloseHandle(hMapFile);

	SetFilePointer(hFile, 12, 0, FILE_CURRENT);
	SetEndOfFile(hFile);

	crcChecker->WriteCRC(hFile);
	CloseHandle(hFile);
	cout << "Messages deleted" << endl;
	return true;
}

// Returns count of the messages in the mailbox with specified name
unsigned long MailSystem::MessageCount(const char* name) {
	if (!loadedCorrectly) {
		cout << "Object loaded incorrectly. Solve problems and try again" << endl;
		return false;
	}

	HANDLE h = OpenFile(name);

	if (h == INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		return 0xffffffff;
	}

	DWORD res = 0;
	DWORD real;

	ReadFile(h, &res, 4, &real, 0);

	CloseHandle(h);

	return res;
}
