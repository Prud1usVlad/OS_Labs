// ConsoleApp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <tchar.h>

typedef void (__stdcall* f_GetKeys)(unsigned __int64* n, unsigned long* e, unsigned __int64* d);
typedef std::vector<unsigned long> (__stdcall* f_EncryptStr)(std::string message, unsigned long e, unsigned __int64 n);
typedef std::string (__stdcall* f_DecryptStr)(std::vector<unsigned long> e_message, unsigned long d, unsigned __int64 n);

void LoadAndPrint(HINSTANCE dll, int id, int buferSize = 100);

int main(int argc, char* argv[])
{
    // Loading library
    HINSTANCE DllInstance = LoadLibrary(TEXT("..\\x64\\Debug\\Lab3.dll"));

    // Checking if library loaded successfully
    if (!DllInstance) {
        std::cout << "Could not load library" << std::endl;
        return EXIT_FAILURE;
    }

    // Getting adreses of functions 
    f_GetKeys GetKeys = (f_GetKeys)GetProcAddress(DllInstance, "GetKeys");
    f_EncryptStr EncryptStr = (f_EncryptStr)GetProcAddress(DllInstance, "EncryptStr");
    f_DecryptStr DecryptStr = (f_DecryptStr)GetProcAddress(DllInstance, "DecryptStr");

    // Checking if functions found
    if (!GetKeys) {
        std::cout << "Could not find function GetKeys" << std::endl;
        return EXIT_FAILURE;
    }
    if (!EncryptStr) {
        std::cout << "Could not find function EncryptStr" << std::endl;
        return EXIT_FAILURE;
    }
    if (!DecryptStr) {
        std::cout << "Could not find function DecryptStr" << std::endl;
        return EXIT_FAILURE;
    }

    // Main program code

    std::string message = "Some simple message";

    long encrypted[32];
    char decrypted[32];
    time_t start, end;
    unsigned long long n = 9173503;
    unsigned long e = 3;
    unsigned long long d = 6111579;

    time(&start);
    GetKeys(&n, &e, &d);
    std::cout << "Open key: {" << e << ", " << n << "} " << std::endl;
    std::cout << "Secret key: {" << d << ", " << n << "} " << std::endl;

    std::cout << "Message: " << message << std::endl;

    std::vector<unsigned long> enc = EncryptStr(message, e, n);

    std::cout << "Encrypted: " << std::endl;
    for (int i = 0; i < enc.size(); i++) {
        std::cout << enc[i] << " ";
    }
    std::cout << std::endl;

    std::string decr = DecryptStr(enc, d, n);
    std::cout << "Decrypted: " << decr << std::endl;
    time(&end);

    std::cout << "Time: " << (double)difftime(end, start) << " seconds" << std::endl;

    // Unloading RSA library 
    FreeLibrary(DllInstance);

    std::cout << "\n\n\n" << " -------- Second Part -------- " << "\n\n\n";

    if (argv[1][0] == '1') {
        std::cout << "English selected" << std::endl;
        DllInstance = LoadLibrary(TEXT("..\\x64\\Debug\\ResourceLibEN.dll"));

        if (DllInstance) {
            TCHAR* str = new TCHAR[100];
            LoadString(DllInstance, 101, str, 100);
            _tprintf(TEXT("Second name: %s\n"), str);
            LoadString(DllInstance, 102, str, 100);
            _tprintf(TEXT("Faculty: %s\n"), str);
            LoadString(DllInstance, 103, str, 100);
            _tprintf(TEXT("Study group: %s\n"), str);
            LoadString(DllInstance, 104, str, 100);
            _tprintf(TEXT("Subject: %s\n"), str);
            _tprintf(TEXT("\n"));
        }
        else {
            std::cout << "Can't load resource dll" << std::endl;
        }
    }
    else if (argv[1][0] == '2') {
        _tsetlocale(LC_ALL, TEXT("Ukrainian"));
        std::cout << "Успiх! Обрано українську" << std::endl;
        DllInstance = LoadLibrary(TEXT("..\\x64\\Debug\\ResourceLibUA.dll"));

        if (DllInstance) {
            TCHAR* str = new TCHAR[100];
            LoadString(DllInstance, 101, str, 100);
            _tprintf(TEXT("Прiзвище: %s\n"), str);
            LoadString(DllInstance, 102, str, 100);
            _tprintf(TEXT("Факультет: %s\n"), str);
            LoadString(DllInstance, 103, str, 100);
            _tprintf(TEXT("Група: %s\n"), str);
            LoadString(DllInstance, 104, str, 100);
            _tprintf(TEXT("Предмет: %s\n"), str);
            _tprintf(TEXT("\n"));
        }
        else {
            std::cout << "Can't load resource dll" << std::endl;
        }
    }
    else {
        std::cout << "Incorrect value of first argument. You should use 1 for English or 2 for Ukrainian" << std::endl;
    }
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
