// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <stdio.h>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        printf("lpReserved = %p\n", lpReserved);

        if (lpReserved == 0) {
            printf("Dynamically loaded library \n");
        }
        else {
            printf("Statically loaded library \n");
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}