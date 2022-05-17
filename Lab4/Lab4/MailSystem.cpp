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
/* example path : .\\dir\\subdir      */
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

		crcChecker->WriteCRC(h);

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

	HANDLE h = CreateFileA(
		path.c_str(),
		FILE_GENERIC_READ | FILE_GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		0,
		0
	);

	if (!CheckFileCRC(h)) { 
		CloseHandle(h);
		return false; 
	}

	DWORD real;
	FileData* fd = new FileData(h);

	unsigned long bytes = strlen(message);
	fd->mailSize += bytes + 4;
	fd->msgCount++;

	if (fd->mailSize > fd->maxSize) {
		cout << "Mailbox: " << name << " is out of memory. Can't add last message";
		CloseHandle(h);
		return false;
	}

	WriteFile(h, &fd->msgCount, 4, &real, NULL);
	WriteFile(h, &fd->mailSize, 4, &real, NULL);

	SetFilePointer(h, -2, NULL, FILE_END);

	WriteFile(h, &bytes, 4, &real, NULL);
	WriteFile(h, message, bytes, &real, NULL);

	crcChecker->WriteCRC(h);

	CloseHandle(h);
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

	HANDLE h = OpenFile(name);

	if (h == INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		return "error";
	}

	DWORD real;
	DWORD count;

	ReadFile(h, &count, 4, &real, 0);

	if (index > count || index <= 0) {
		cout << "Invalid index" << endl;
		CloseHandle(h);
		return "error";
	}

	SetFilePointer(h, 12, 0, FILE_BEGIN);
	DWORD size = 0;

	for (int i = 1; i < index; ++i) {
		ReadFile(h, &size, 4, &real, 0);
		SetFilePointer(h, size, 0, FILE_CURRENT);
	}

	ReadFile(h, &size, 4, &real, NULL);
	char* data = new char[size + 1];
	ReadFile(h, data, size, &real, NULL);
	data[size] = '\0';


	CloseHandle(h);
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

	HANDLE h = OpenFile(name);

	if (h == INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		return "error";
	}

	FileData* fd = new FileData(h);

	if (index > fd->msgCount || index <= 0) {
		cout << "Invalid index" << endl;
		CloseHandle(h);
		return "error";
	}

	DWORD real;
	unsigned long shift = 12;
	unsigned long toDeleteBytes = fd->mailSize;
	unsigned long toMoveBytes = 0;


	// Iterating to the nedded message
	SetFilePointer(h, 12, 0, FILE_BEGIN);

	for (int i = 1; i < index; ++i) {
		DWORD size = 0;
		ReadFile(h, &size, 4, &real, 0);
		SetFilePointer(h, size, 0, FILE_CURRENT);
		shift += size + 4;
	}

	// If deleting not last message
	if (index < fd->msgCount) {
		ReadFile(h, &toDeleteBytes, 4, &real, 0);
		SetFilePointer(h, toDeleteBytes, 0, FILE_CURRENT);
		toDeleteBytes += 4;

		toMoveBytes = fd->mailSize - shift - toDeleteBytes + 12;
		char* data = new char[toMoveBytes];

		ReadFile(h, data, toMoveBytes, &real, 0);
		SetFilePointer(h, shift, 0, FILE_BEGIN);
		WriteFile(h, data, toMoveBytes, &real, 0);

		delete[] data;
	}
	
	SetEndOfFile(h);

	SetFilePointer(h, 0, 0, FILE_BEGIN);

	DWORD newData = fd->msgCount - 1; 
	WriteFile(h, &newData, 4, &real, 0);
	newData = fd->mailSize - toDeleteBytes;
	WriteFile(h, &newData, 4, &real, 0);

	crcChecker->WriteCRC(h);
	CloseHandle(h);
	cout << "Message deleted" << endl;
	return true;
}

// Deletes all mesages from file
bool MailSystem::Clear(const char* name) {
	if (!loadedCorrectly) {
		cout << "Object loaded incorrectly. Solve problems and try again" << endl;
		return false;
	}

	HANDLE h = OpenFile(name);

	if (h == INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		return false;
	}

	DWORD real;
	DWORD data = 0;

	WriteFile(h, &data, 4, &real, 0);
	WriteFile(h, &data, 4, &real, 0);
	SetFilePointer(h, 4, 0, FILE_CURRENT);
	SetEndOfFile(h);

	crcChecker->WriteCRC(h);
	CloseHandle(h);
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
