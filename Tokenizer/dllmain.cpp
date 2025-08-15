#include "pch.h"
#include <locale>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Set the global locale to the user's default system locale.
        // This is essential for iswalpha, iswalnum, etc., to correctly
        // classify Unicode characters.
        try {
            std::locale::global(std::locale(""));
        }
        catch (...) {
            // Failsafe in case the locale cannot be set.
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

