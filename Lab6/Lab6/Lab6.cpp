// Lab6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <algorithm>
#include <windows.h>
#include "CustomStack.h"
#include <list>
#include "LRUcache.h"

using namespace std;

int Optimal(const int* sequence, int count);
int FIFO(const int* sequence, int count);
int Counters(const int* sequence, int count);
int Clock(const int* sequence, int count);
int LRU(const int* sequence, int count);

LPVOID SavePassword();
bool CheckPassword(LPVOID address);
void FlushPassword(LPVOID address);


// Constants for hash function
#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

unsigned HashString(const char* s);
int IndexOf(const int* arr, int size, int element, int startIndex = 0);

int main()
{
    // Arrays that indicates sequence of use of pages
    int sequence[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

    // Calling functions to perform pages perlacements algorithms

    /*printf("Optimal algotithm: \n");
    printf("Number of replacements: %i\n\n", Optimal(sequence, 12));

    printf("FIFO algotithm: \n");
    printf("Number of replacements: %i\n\n", FIFO(sequence, 12));

    printf("Counters algotithm: \n");
    printf("Number of replacements: %i\n\n", Counters(sequence, 12));

    printf("Clock algotithm: \n");
    printf("Number of replacements: %i\n\n", Clock(sequence, 12));

    printf("LRU algotithm: \n");
    printf("Number of replacements: %i\n\n", LRU(sequence, 12));*/

    /*LPVOID address = SavePassword();
    CheckPassword(address);
    FlushPassword(address);*/


    //CustomStack * stack = new CustomStack(10000);

    //for (int i = 0; i < 2000; ++i) {
    //    stack->push(i);
    //}

    //for (int i = 0; i < 2000; ++i) {
    //    cout << "Value" << stack->pop() << endl;
    //}

    //delete stack;


   LRUcache cache(3);

    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3); 
    cache.put(4, 4);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(5, 5);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    cache.put(4, 4);
    cache.put(5, 5);

    for (CacheFrame frame : cache.queue) {
        cout << frame.value << " ";
    }


}

// Returns Amount of replacements
int Optimal(const int* sequence, int count) {
    // Memory model
    int memory[4] = { 0, 0, 0, 0 };
    int busyMemory = 0;
    int replacements = 0;

    for (int i = 0; i < count; ++i) {
        int page = sequence[i];

        if (busyMemory == 4) {

            // Checking if new page is in the list
            bool contains = IndexOf(memory, 4, page) != -1;

            // Analizing wich element should we replace
            if (!contains) {
                int maxDistance = INT16_MIN;
                int indexToReplace = memory[0];

                for (int j = 0; j < 4; ++j) {
                    int distance = IndexOf(sequence, count, memory[j], i + 1);

                    // Page doesn't ment to be used in future 
                    if (distance == -1) {
                        indexToReplace = j;
                        break;
                    }
                    else if (distance > maxDistance) {
                        maxDistance = distance;
                        indexToReplace = j;
                    }
                }

                memory[indexToReplace] = page;
                replacements++;
                printf("Page %i added to the memory :     [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
            }
            else {
                printf("Page %i already in the memory :   [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
            }
        }
        else {
            memory[busyMemory++] = page;
            printf("Page %i added to the memory :     [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
        }
    }

    return replacements;
}

// Returns Amount of replacements
int FIFO(const int* sequence, int count) {
    // Memory model
    int memory[4] = { 0, 0, 0, 0 };
    int busyMemory = 0;
    // Index of the page that has been added before all
    int queueSatartIndex = 0;
    int replacements = 0;

    for (int i = 0; i < count; ++i) {
        int page = sequence[i];

        if (busyMemory == 4) {

            // Checking if new page is in the list
            bool contains = IndexOf(memory, 4, page) != -1;

            // Analizing wich element should we replace
            if (!contains) {
                memory[queueSatartIndex] = page;
                // Shifting index of the queue start
                queueSatartIndex = (queueSatartIndex + 1) % 4;
                replacements++;
                printf("Page %i added to the memory :     [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
            }
            else {
                printf("Page %i already in the memory :   [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
            }
        }
        else {
            memory[busyMemory++] = page;
            printf("Page %i added to the memory :     [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
        }
    }

    return replacements;
}

// Returns Amount of replacements
int Counters(const int* sequence, int count) {
    // Memory model
    int memory[4] = { 0, 0, 0, 0 };
    int busyMemory = 0;
    int counters[4] = { 0, 0, 0, 0, };
    int toReplaceIndex = 0;
    int replacements = 0;

    for (int i = 0; i < count; ++i) {
        int page = sequence[i];

        int index = IndexOf(memory, 4, page);

        // If new page added 
        if (index == -1) {
            memory[toReplaceIndex] = page;
            printf("Page %i added to the memory :     [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);

            if (busyMemory == 4) {
                ++replacements;
            }
            else {
                ++busyMemory;
            }
        }
        // If page already in memory
        else {
            printf("Page %i already in the memory :   [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
            toReplaceIndex = index;
        }

        // Mark current node as used
        counters[toReplaceIndex] = -1;

        int maxCounter = INT16_MIN;
        int newIndex = 0;

        // Incrementing each counter and finding 
        // new index that shold be replaced
        for (int j = 0; j < 4; ++j) {
            ++counters[j];

            if (counters[j] > maxCounter) {
                newIndex = j;
                maxCounter = counters[j];
            }
        }

        toReplaceIndex = newIndex;
    }

    return replacements;
}

// Returns Amount of replacements
int Clock(const int* sequence, int count) {
    // Memory model
    int memory[4] = { 0, 0, 0, 0 };
    bool access[4] = { 0, 0, 0, 0 };
    int clockHand = 0;
    int replacements = 0;

    for (int i = 0; i < count; ++i) {
        int page = sequence[i];
        bool found = false;

        // Finding Cycle
        for (int j = 0; j < 4; ++j) {
            // If found 
            if (memory[clockHand] == page) {
                found = true;
                // Set acces byte to true
                access[clockHand] = true;
                // Move clock hand
                clockHand = (clockHand + 1) % 4;
                // And quit the loop  
                printf("Page %i already in the memory :   [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
                break;
            }

            clockHand = (clockHand + 1) % 4;
        }

        // Setting cycle
        if (!found) {
            for (int j = 0; j < 5; ++j) {

                // Change acces byte
                if (access[clockHand]) {
                    access[clockHand] = false;
                }
                // If acces byte = 0, we can place page 
                else {
                    memory[clockHand] = page;
                    access[clockHand] = true;
                    clockHand = (clockHand + 1) % 4;
                    printf("Page %i added to the memory :     [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
                    ++replacements;
                    break;
                }

                clockHand = (clockHand + 1) % 4;
            }
        }

    }

    return replacements - 4;
}

// Returns Amount of replacements
int LRU(const int* sequence, int count) {
    // Memory model
    int memory[4] = { 0, 0, 0, 0 };
    int busyMemory = 0;
    // Index of the page that has been added before all
    int replacements = 0;

    for (int i = 0; i < count; ++i) {
        int page = sequence[i];

        if (busyMemory == 4) {

            // Checking if new page is in the list
            int index = IndexOf(memory, 4, page);

            if (index == -1) {

                for (int j = 0; j < 3; ++j) {
                    memory[j] = memory[j + 1];
                }

                memory[3] = page;

                printf("Page %i added to the memory :     [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
            }
            else {
                for (int j = index; j < 3; ++j) {
                    swap(memory[j], memory[j + 1]);
                }

                printf("Page %i already in the memory :   [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
                ++replacements;
            }
        }
        else {
            memory[busyMemory++] = page;
            printf("Page %i added to the memory :     [ %i, %i, %i, %i ] \n", page, memory[0], memory[1], memory[2], memory[3]);
        }
    }

    return replacements;
}

int IndexOf(const int* arr, int size, int element, int startIndex) {
    for (int i = startIndex; i < size; ++i) {
        if (element == arr[i]) {
            return i;
        }
    }

    return -1;
}

unsigned HashString(const char* s)
{
    unsigned h = FIRSTH;
    while (*s) {
        h = (h * A) ^ (s[0] * B);
        s++;
    }
    return h % C;
}

// Returns addres and size of the page in memory where password is stored  
LPVOID SavePassword() {

    char pass[64];
    LPVOID memoryAddres = 0;

    while (true) {
        printf("Write password. It should be longer than 8 characters contain \nat least 1 capital letter number and special char '|.,$#@%*'. \n To exit function type 'exit'.\n");
        cin >> pass;

        if (pass == "exit") {
            printf("Exit function...");
            return 0;
        }

        if (strlen(pass) < 8) {
            continue;
        }

        bool hasNumber = false;
        bool hasCapitalLetter = false;
        bool hasSpecialChar = false;

        for (int i = 0; i < strlen(pass); ++i) {
            char c = pass[i];

            hasNumber = hasNumber || isdigit(c);
            hasCapitalLetter = hasCapitalLetter || isupper(c);
            hasSpecialChar = hasSpecialChar ||
                c == '|' || c == '.' || c == ',' || c == '$' ||
                c == '#' || c == '@' || c == '%' || c == '*';
        }

        if (hasNumber && hasCapitalLetter && hasSpecialChar) {
            char confirm[64];
            printf("Please write password again for confirmation\n");
            cin >> confirm;
            if (strcmp(pass, confirm) == 0) {
                printf("Password accepted!\n");
                break;
            }
            else {
                printf("Confirmation error.\n");
            }
        }
    }

    memoryAddres = VirtualAlloc(0, sizeof(DWORD), MEM_COMMIT, PAGE_READWRITE);

    DWORD passHash = HashString(pass);

    memcpy(memoryAddres, &passHash, sizeof(DWORD));

    VirtualLock(memoryAddres, 4096);

    return memoryAddres;
}

bool CheckPassword(LPVOID address) {

    char input[64];
    DWORD inputHash;
    DWORD passHash;

    memcpy(&passHash, address, sizeof(DWORD));

    while (true) {
        printf("Write password.\n");
        cin >> input;

        if (input == "exit") {
            printf("Exit function...");
            return false;
        }

        inputHash = HashString(input);

        if (inputHash != passHash) {
            printf("Incorrecet password\n");
        }
        else {
            printf("Success. Access granted\n");
            return true;
        }
    }


}

void FlushPassword(LPVOID address) {
    DWORD junkData = 0;

    memcpy(address, &junkData, sizeof(DWORD));

    VirtualUnlock(address, 4096);
    printf("Password deleted. Page can be unloaded");
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
