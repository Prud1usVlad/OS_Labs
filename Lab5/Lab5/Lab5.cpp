// Lab5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include "Block.h"

using namespace std;

void OutputSystemInfo();
vector<Block*>* GetBlocksList(bool onlyFreeBlocks = true, unsigned long maxSize = 10000);
Block* CustomAlloc(DWORD size, vector<Block*>* freeList, vector<Block*>* busyList);
Block* CustomFree(DWORD size, vector<Block*>* freeList, vector<Block*>* busyList);

int main()
{

    // Outputing system info tasks 1 - 4
    OutputSystemInfo();

    // Building lists of memory blocks (for example free) tasks 6 - 7 
    vector<Block*>* freeBlocks = GetBlocksList();

    // Allocating memory block vith size 10000 bits 
    HANDLE hProcess = GetCurrentProcess();
    LPVOID res = VirtualAllocEx(hProcess, 0, 10000, MEM_COMMIT, PAGE_READWRITE);
    if (res != NULL) cout << "\nMemory Allocated\n Adress: " << (DWORD)res << endl;
    else cout  << "Error:" << GetLastError() << endl;
    
    // Building new list of free memory blocks 
    vector<Block*>* newFreeBlocks = GetBlocksList();

    // Outputing data to compare old and new lists
    cout << "Total free blocks before allocation: " << freeBlocks->size() << endl;
    cout << "Total free blocks after allocation: " << newFreeBlocks->size() << endl;

    for (int i = 0; i < freeBlocks->size(); ++i) {
        Block* fromOld = freeBlocks->at(i);
        Block* fromNew = newFreeBlocks->at(i);

        cout << "----------------------------------------------" << endl;
        cout << "OldData: Adress :" << fromOld->dwAdress << ", Size: " << fromOld->dwSize << ", State: " << fromOld->state << ", Protection: " << fromOld->protect << endl;
        cout << "NewData: Adress :" << fromNew->dwAdress << ", Size: " << fromNew->dwSize << ", State: " << fromNew->state << ", Protection: " << fromNew->protect << endl;
    }

    // Building new list of busy blocks 
    vector<Block*>* busyBlocks = GetBlocksList(false);

    // Calling custom allocation and free methods task 9
    Block* newBlock1 = CustomAlloc(100000, newFreeBlocks, busyBlocks);
    if (newBlock1 != NULL)
        cout << "\nMemory allocated. New Block added: adress: " << newBlock1->dwAdress << " size: " << newBlock1->dwSize << endl;
    else
        cout << "\nCan't find block of memory to allocate" << endl;
    Block* newBlock2 = CustomAlloc(1000, newFreeBlocks, busyBlocks);
    if (newBlock2 != NULL)
        cout << "\nMemory allocated. New Block added: adress: " << newBlock2->dwAdress << " size: " << newBlock2->dwSize << endl;
    else
        cout << "\nCan't find block of memory to allocate" << endl;
    Block* newBlock3 = CustomFree(4096, newFreeBlocks, busyBlocks);
    if (newBlock3 != NULL)
        cout << "\nMemory freed. New Block added: adress: " << newBlock3->dwAdress << " size: " << newBlock3->dwSize << endl;
    else
        cout << "\nCan't find block of memory to free" << endl;
    Block* newBlock4 = CustomFree(20000, newFreeBlocks, busyBlocks);
    if (newBlock4 != NULL)
        cout << "\nMemory freed. New Block added: adress: " << newBlock4->dwAdress << " size: " << newBlock4->dwSize << endl;
    else
        cout << "\nCan't find block of memory to free" << endl;
        

    cout << "\nFree blocks list:\n" << endl;

    for (int i = 0; i < freeBlocks->size(); ++i) {
        Block* block = newFreeBlocks->at(i);

        cout << "Adress :" << block->dwAdress << "   Size: " << block->dwSize << endl;
    }

    cout << "\nBusy blocks list:\n" << endl;

    for (int i = 0; i < busyBlocks->size(); ++i) {
        Block* block = busyBlocks->at(i);

        cout << "Adress :" << block->dwAdress << "   Size: " << block->dwSize << endl;
    }
}

void OutputSystemInfo() {
    cout << "\n --- System info ---\n" << endl;

    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);

    cout << "dwPageSize = " << SystemInfo.dwPageSize << endl;
    cout << "dwAllocationGranularity = " << SystemInfo.dwAllocationGranularity << endl;
    cout << "lpMinimumApplicationAddress = " << SystemInfo.lpMinimumApplicationAddress << endl;
    cout << "lpMaximumApplicationAddress = " << SystemInfo.lpMaximumApplicationAddress << endl;

    cout << "\n --- Memory info ---\n" << endl;

    MEMORYSTATUSEX MemoryStatus;
    MemoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&MemoryStatus);

    cout << "ullAvailPhys = " << MemoryStatus.ullAvailPhys << endl;
    cout << "ullAvailVirtual = " << MemoryStatus.ullAvailVirtual << endl;
    cout << "ullAvailPageFile = " << MemoryStatus.ullAvailPageFile << endl;
    cout << "ullTotalPhys = " << MemoryStatus.ullTotalPhys << endl;
    cout << "ullTotalVirtual = " << MemoryStatus.ullTotalVirtual << endl;
    cout << "ullTotalPageFile = " << MemoryStatus.ullTotalPageFile << endl;
}

vector<Block*>* GetBlocksList(bool onlyFreeBlocks, unsigned long maxSize) {

    vector<Block*>* res = new vector<Block*>();

    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    MEMORY_BASIC_INFORMATION Buffer;
    HANDLE hProcess = GetCurrentProcess();
    DWORD dwAdress = (DWORD)SystemInfo.lpMinimumApplicationAddress;

    while (dwAdress < (DWORD)SystemInfo.lpMaximumApplicationAddress && res->size() <= maxSize)
    {
        DWORD dwSize = VirtualQueryEx(hProcess, (const void*)dwAdress,
            &Buffer, sizeof(MEMORY_BASIC_INFORMATION));

        if (onlyFreeBlocks && Buffer.State == MEM_FREE || !onlyFreeBlocks && Buffer.State != MEM_FREE)
            res->push_back(new Block(dwAdress, Buffer.RegionSize, Buffer.State, Buffer.AllocationProtect));

        dwAdress += Buffer.RegionSize;

    }

    CloseHandle(hProcess);
    return res;
}

Block* CustomAlloc(DWORD size, vector<Block*>* freeList, vector<Block*>* busyList) {

    DWORD minAdress = 0;
    DWORD minSize = UINT32_MAX;
    DWORD index = 0;

    for (int i = 0; i < freeList->size(); ++i) {
        Block* current = freeList->at(i);

        if (current->dwSize < minSize && current->dwSize >= size) {
            minSize = current->dwSize;
            minAdress = current->dwAdress;
            index = i;
        }
    }

    if (minSize == UINT32_MAX && minAdress == 0) {
        return NULL;
    }

    if (size == minSize) {
        freeList->erase(freeList->begin() + index);
    }
    else {
        Block* block = freeList->at(index);
        block->dwAdress += size;
        block->dwSize -= size;
    }

    for (int i = 0; i < busyList->size(); ++i) {
        index = i;
        if (busyList->at(i)->dwAdress > minAdress) {
            break;
        }
    }

    Block* res = new Block(minAdress, size);
    busyList->insert(busyList->begin() + index, res);
    return res;
}

Block* CustomFree(DWORD size, vector<Block*>* freeList, vector<Block*>* busyList) {

    DWORD minAdress = 0;
    DWORD minSize = UINT32_MAX;
    DWORD index = 0;

    for (int i = 0; i < busyList->size(); ++i) {
        Block* current = busyList->at(i);

        if (current->dwSize < minSize && current->dwSize >= size) {
            minSize = current->dwSize;
            minAdress = current->dwAdress;
            index = i;
        }
    }

    if (minSize == UINT32_MAX && minAdress == 0) {
        return NULL;
    }

    if (size == minSize) {
        busyList->erase(busyList->begin() + index);
    }
    else {
        Block* block = busyList->at(index);
        block->dwAdress += size;
        block->dwSize -= size;
    }

    for (int i = 0; i < freeList->size(); ++i) {
        index = i;
        if (freeList->at(i)->dwAdress > minAdress) {
            break;
        }
    }

    Block* res = new Block(minAdress, size);
    freeList->insert(freeList->begin() + index, res);
    return res;
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
