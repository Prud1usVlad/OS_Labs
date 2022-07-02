// Lab7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <stdio.h> 
#include <tchar.h> 
#include <locale.h>
#include <TlHelp32.h>
#include <windows.h>
#include <psapi.h>


// Macro for short-hand calls of functions to cteate processes
#define CustomCreateProcess(commandLine, startupInfo, processInfo) \
    CreateProcess(0, commandLine, 0, 0, false, 0, 0, 0, startupInfo, processInfo)
#define CustomCreateSuspendedProcess(commandLine, startupInfo, processInfo) \
    CreateProcess(0, commandLine, 0, 0, false, CREATE_SUSPENDED, 0, 0, startupInfo, processInfo)

using namespace std;

void OutputDlls(DWORD id);

int main()
{
    // Creating snapshot and prepering data structure 
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 processentry32;
    processentry32.dwSize = sizeof(PROCESSENTRY32);

    cout << "Process info: " << endl;

    // Findidng first process
    Process32First(hSnapshot, &processentry32);

    do {

        // Outputing data about processes
        cout << "~~~~~~~~~~~~~" << endl;
        wcout << "Process name: " << processentry32.szExeFile << endl;
        wcout << "Process threads: " << processentry32.cntThreads << endl;
        cout << "Dlls of the process: " << endl;
        OutputDlls(processentry32.th32ProcessID);

    } while (Process32Next(hSnapshot, &processentry32));

    CloseHandle(hSnapshot);
}

// Iterating and outputing names of libraries
// used by the process with specified ID 
void OutputDlls(DWORD id) {

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
    MODULEENTRY32 pModule;
    pModule.dwSize = sizeof(MODULEENTRY32);

    if (hSnap == INVALID_HANDLE_VALUE)
    {
        cout << "No dll info" << endl;
        return;
    }

    if (!Module32First(hSnap, &pModule))
    {
        cout << "No dll info" << endl;
        CloseHandle(hSnap);      
        return;
    }

    do {
        wcout << "\t" << pModule.szModule << endl;
    } while (Module32Next(hSnap, &pModule));

    CloseHandle(hSnap);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
