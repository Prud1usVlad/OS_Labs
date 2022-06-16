#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <fileapi.h>
#include <format>
#include "CRCChecker.h"

using namespace std;

#ifndef MailSys
#define MailSys
class MailSystem {
public:

	MailSystem(const char* directoryPath = 0);
	bool CreateMailbox(const char* name, unsigned int size = INT16_MAX);
	unsigned long GetMailSystemInfo();
	bool AddMessage(const char* mailboxName, const char* message);
	string ReadMessage(const char* mailboxName, unsigned long index);
	string ReadAndDelete(const char* mailboxName, unsigned long index);
	bool Clear(const char* mailboxName);
	bool Delete(const char* mailboxName, unsigned long index);
	unsigned long MessageCount(const char* name);

private:
	string dir;
	int mailboxCount;
	bool loadedCorrectly;


	WIN32_FIND_DATAA lastFound;
	CRCChecker * crcChecker;

	bool CheckFileCRC(HANDLE hFile);
	HANDLE OpenFile(const char* name);
};
#endif // !MailSys

