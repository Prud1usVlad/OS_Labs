// NotepadLaunch.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <stdio.h> 
#include <conio.h> 
#include <tchar.h> 
#include <locale.h>

#define CustomCreateProcess(commandLine, startupInfo, processInfo) \
    CreateProcess(0, commandLine, 0, 0, false, 0, 0, 0, startupInfo, processInfo)
#define CustomCreateSuspendedProcess(commandLine, startupInfo, processInfo) \
    CreateProcess(0, commandLine, 0, 0, false, CREATE_SUSPENDED, 0, 0, startupInfo, processInfo)

using namespace std;

int main()
{
    cout << "Launching notepad..." << endl;

    PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo;

    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    TCHAR processName[] = TEXT("notepad.exe");
    if (!CustomCreateProcess(processName, &startupInfo, &processInfo)) {
        cout << "Error occured while notepad.exe is started" << endl;
        return -1;
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);
    return 1;
}