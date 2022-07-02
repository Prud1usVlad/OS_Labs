

#include <iostream>
#include <windows.h>
#include <stdio.h> 
#include <conio.h> 
#include <tchar.h> 
#include <locale.h>
#include <string>

#define CustomCreateProcess(commandLine, startupInfo, processInfo) \
    CreateProcess(0, commandLine, 0, 0, false, 0, 0, 0, startupInfo, processInfo)
#define CustomCreateSuspendedProcess(commandLine, startupInfo, processInfo) \
    CreateProcess(0, commandLine, 0, 0, false, CREATE_SUSPENDED, 0, 0, startupInfo, processInfo)

using namespace std;

// Main programm (program number 3 from task).
// Calls NotepadLaunch and FilesInfo processes  
int main()
{
    cout << "Main process started" << endl;

    _tsetlocale(LC_ALL, _T("Ukrainian"));

    SYSTEMTIME systemTime; 
    FILETIME fileTime; 

    GetLocalTime(&systemTime); 
    SystemTimeToFileTime(&systemTime, &fileTime); 
    unsigned _int64 startTime = ((unsigned _int64)fileTime.dwHighDateTime << 32) | fileTime.dwLowDateTime;

    string timeStr = to_string(startTime);
    SetEnvironmentVariableA("StartTime", timeStr.c_str());

    PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo;

    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    TCHAR processName[] = TEXT("NotepadLaunch.exe");
    if (!CustomCreateProcess(processName, &startupInfo, &processInfo)) {
        cout << "Error occured while NotepadLaunch.exe is started" << endl; 
        return -1; 
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);

    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    
    TCHAR secondProcessName[] = TEXT("FilesInfo.exe");

    if (!CustomCreateProcess(secondProcessName, &startupInfo, &processInfo)) {
        cout << "Error occured while FilesInfo.exe is started" << endl;
        return -1;
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);

    cout << "Main process ended" << endl;
}

