//
// Created by alteik on 29.10.2025.
//

#include "vcruntime.hpp"

typedef size_t (RUNTIME_CALL *_CxxFrameHandler4_t)(void*, size_t, void*, void*);

static HMODULE gRuntime = nullptr;
static _CxxFrameHandler4_t gCxxFrameHandler4 = nullptr;

#pragma comment(linker,"/export:__CxxFrameHandler4=_CxxFrameHandler4")
RUNTIME_EXPORT size_t RUNTIME_CALL _CxxFrameHandler4(void* pExcept, size_t RN, void* pContext, void*pDC)
{
    if (gCxxFrameHandler4) return gCxxFrameHandler4(pExcept, RN, pContext, pDC);
    return 1;
}

#pragma comment(linker,"/export:__NLG_Dispatch2=_NLG_Dispatch2")
RUNTIME_EXPORT void RUNTIME_CALL _NLG_Dispatch2() {}

#pragma comment(linker,"/export:__NLG_Return2=_NLG_Return2")
RUNTIME_EXPORT void RUNTIME_CALL _NLG_Return2() {}

namespace proxy
{
    inline void crash()
    {
        std::exit(-1);
    }

    HMODULE getLocalVCRuntime()
    {
        return LoadLibraryEx(_T(VC_RUNTIME_LOCAL_LIB), nullptr, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
    }

    HMODULE getSystemVCRuntime()
    {
        size_t length = 0;
        static wchar_t runtimePath[MAX_PATH];

        if (GetSystemDirectoryW(runtimePath, MAX_PATH - 1))
        {
            auto runtime = _T(VC_RUNTIME_LIB);
            wchar_t* endPath = runtimePath;

            while (*endPath++) ++length;

            if (length + ARRAYSIZE(VC_RUNTIME_LIB) > MAX_PATH)
                return FALSE;

            *(--endPath)++ = '\\';
            while ((*endPath++ = *runtime++)) {}

            return LoadLibraryExW(runtimePath, nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        }
        return nullptr;
    }

    void init()
    {
        if (!gRuntime)
        {
            gRuntime = getLocalVCRuntime();
            if (!gRuntime) gRuntime = getSystemVCRuntime();

            if (gRuntime)
            {
                gCxxFrameHandler4 = (_CxxFrameHandler4_t)GetProcAddress(gRuntime, "__CxxFrameHandler4");
            }
        }

        if (!gCxxFrameHandler4) crash();
    }
    void shutdown()
    {
        if (gRuntime)
        {
            FreeLibrary(gRuntime);
            gCxxFrameHandler4 = nullptr;
            gRuntime = nullptr;
        }
    }
}