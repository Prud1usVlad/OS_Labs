// FilesInfo.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <windows.h>
#include <stdio.h> 
#include <conio.h> 
#include <tchar.h> 
#include <locale.h>
#include <string>
#include <vector>

using namespace std;

const char* GetEncoding(FILE* File);

int main(int argc, char* argv[])
{
    cout << "Geathering files info..." << endl;

    WIN32_FIND_DATA findFileData;
    FILETIME fileTime;
    HANDLE hFile = FindFirstFile(_T("*.txt"), &findFileData);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Files not found");
        CloseHandle(hFile);
        return -1;
    }

    char timeStr[513];
    char* eptr;
    GetEnvironmentVariableA("StartTime", timeStr, 512);
    
    unsigned _int64 startTime = strtoull(timeStr, &eptr, 10);

    while (true) {

        FileTimeToLocalFileTime(&findFileData.ftCreationTime, &fileTime);
        unsigned _int64 createTime = ((unsigned _int64)fileTime.dwHighDateTime << 32) | fileTime.dwLowDateTime;

        // If file created now
        //if (createTime > startTime) {
        if (true) {
            // Outputing file data
            cout << "~~~~~~~~~~~~~" << endl;
            wcout << "File: " << findFileData.cFileName << endl;
            unsigned _int64 fileSize = ((unsigned _int64)findFileData.nFileSizeHigh << 32) | findFileData.nFileSizeLow;
            cout << "Size: " << fileSize << endl;

            cout << "Strings:" << endl;

            FILE* stream;

            if (_wfopen_s(&stream, findFileData.cFileName, L"r") == 0) {
                int lineCount = 1;
                int charCount = 2;

                // File has single-byte encoding
                if (GetEncoding(stream) == "ASCII") {
                    char line[1024];

                    // Reading file line by line
                    while (fgets(line, 1024, stream) != NULL) {
                        charCount += strlen(line);

                        // Compensate removed carriage return symbol if there is end of file
                        if (feof(stream)) charCount++;

                        // Removing carriage return symbol from result in 'charCount - 1'
                        cout << "\tLine " << lineCount++ << ", characters: " << charCount - 1 << endl;
                        charCount = 0;
                        memset(&line, 0, 1024);
                    }
                }
                // File is UTF16
                else {
                    wchar_t character;
                    // Iterating till end of file is found
                    while (!feof(stream))
                    {
                        character = getwc(stream);

                        // If carriage return found
                        if (character == '\r') {

                            // Removing carriage return symbol from result in 'charCount - 2'
                            cout << "\tLine " << lineCount++ << ", characters: " << (charCount - 2) / 2 << endl;
                            charCount = 0;
                        }
                        else {
                            ++charCount;
                        }
                    }

                    cout << "\tLine " << lineCount++ << ", characters: " << (charCount - 3) / 2 << endl;

                }
               
                fclose(stream);
            }
            // Occured error
            else {
                cout << "Error while opening file";
            }
        }

        // Finding next file, if failed brake the loop
        if (!FindNextFile(hFile, &findFileData)) break;
    }

}

const char* GetEncoding(FILE* File)
{

    char first = 100;
    char second = 100;

    first = getc(File);
    second = getc(File);

    if (first == -2 && second == -1)
    {
        return "UTF16BE";
    }
    else if (first == -1 && second == -2)
    {
        return "UTF16LE";
    }
    else
    {
        return "ASCII";
    }
}

