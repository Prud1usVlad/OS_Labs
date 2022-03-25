#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <locale.h>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <codecvt>

#ifdef UNICODE
typedef		wchar_t	TCHAR;
#define	TEXT(a)	L##a
#else
typedef 	char	   	TCHAR;
#define 	TEXT(a)	a
#endif

int compare(const void* a, const void* b);
void output6_8(char* input, wchar_t* w_input, TCHAR* enc, int size);
void output9_11(char* input, wchar_t* w_input);
void output12(std::string enc);
std::string readFile(const char* filename);
std::wstring readFile_W(const char* filename);

int main()
{
    // seting locale
    _tsetlocale(LC_ALL, TEXT("Ukrainian"));

    // input data (generic format)
    TCHAR* t_input = new TCHAR[128];
    _tcscpy(t_input, TEXT("\n\tПрудiус Владислав Юрiйович \n\tПрудiус Юрiй Володимирович \n\tПрудiус Свiтлана Сергiївна"));

    // input data (ASCII format)
    char* input = new char[128];
    strcpy(input, "\n\tПрудiус Владислав Юрiйович \n\tПрудiус Юрiй Володимирович \n\tПрудiус Свiтлана Сергiївна");
   
    // input data (UNICODE format)
    wchar_t* w_input = new wchar_t[strlen(input) + 1];
    MultiByteToWideChar(CP_ACP, 0, input, -1, w_input, strlen(input) + 1);

    // sympol size
    int size = sizeof(TCHAR);

    // used encoding
    TCHAR* enc = new TCHAR[16];
    if (size == 1) {
        _tcscpy(enc, TEXT("ASCII"));
    }
    else {
        _tcscpy(enc, TEXT("UNICODE"));
    }

    // final calls of functions

    //output6_8(input, w_input, enc, size);

    //output9_11(input, w_input);

    output12("UTF-8");
}



// output function for tasks from 6 to 8
void output6_8(char* input, wchar_t* w_input, TCHAR* enc, int size) {
    std::cout << "Розмiр одного символу: " << size * 8 << " бiт" << std::endl;
    _tprintf(TEXT("Викоритстане кодування: %s"), enc);
    std::wcout << L"\nВивiд на стандартний потiк виведення ASCII рядка: " << std::endl;
    std::cout << input << std::endl;
    std::cout << "\nВивiд на стандартний потiк виведення UNICODE рядка: " << std::endl;
    std::wcout << w_input << std::endl;
    std::cout << "\nВивiд в консоль UNICODE рядка за допомогою функцiї _tprintf()" << std::endl;
    _tprintf(TEXT("%s"), w_input);
    std::cout << "\nВивiд тексту у вiкно повiдомлення" << std::endl;
    MessageBoxW(0, w_input, L"Вивід", MB_OK);
}

// delegate function for comparison in sort functions
int compare(const void* a, const void* b)
{
    return (*(wchar_t*)a - *(wchar_t*)b);
}

// output function for tasks from 9 to 11
void output9_11(char* input, wchar_t* w_input) {

    // sorting string w_input, using qsort/sort functions
    qsort(w_input, wcslen(w_input), sizeof(wchar_t), compare);
    //std::sort(w_input, w_input + (wcslen(w_input)));

    std::cout << "\nВивiд в консоль вiдсортованого UNICODE рядка" << std::endl;
    std::wcout << w_input << std::endl;

    // translation UNICODE string into ASCII string
    WideCharToMultiByte(CP_ACP, 0, w_input, -1, input, wcslen(w_input), "", NULL);

    std::cout << "\nВивiд в консоль вiдсортованого UNICODE рядка, що був перетворений назад в ASCII" << std::endl;
    std::cout << input << std::endl;
}

// function wich performs additional 12 task
void output12(std::string enc) {

    char file[] = "text.txt";

    if (enc == "ASCII") {
        std::string text;
        text = readFile(file);
        std::cout << text << std::endl;

        std::reverse(text.begin(), text.end());
        std::cout << text << std::endl;

        std::ofstream out;
        out.open(file, std::ios::app);

        out << "\n" << text << std::endl;
        out.close();
    }
    else if (enc == "UTF-8") {
        std::wstring w_text;
        w_text = readFile_W(file);
        std::wcout << w_text << std::endl;
        
        std::reverse(w_text.begin(), w_text.end());
        std::wcout << w_text << std::endl;

        std::wofstream out;
        out.open(file, std::ios::app);
        out.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

        out << "\n" << w_text << std::endl;
        out.close();
    }
}

// read from file with ascii encoding
std::string readFile(const char* filename) {
    std::fstream fs(filename);
    fs.imbue(std::locale("Ukrainian"));
    std::stringstream wss;
    wss << fs.rdbuf();
    return wss.str();
}

// read from file with utf-8 encoding
std::wstring readFile_W(const char* filename) {
    std::wfstream wif(filename);
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}