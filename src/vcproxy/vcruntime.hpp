//
// Created by alteik on 29.10.2025.
//

#pragma once

#define VCRUNTUME_1_LOADER

#define  RUNTIME_EXPORT			extern "C"
#define  RUNTIME_CALL			__fastcall

#define VC_RUNTIME_LIB			"vcruntime140_1.dll"
#define VC_RUNTIME_LOCAL_LIB	"vcruntime140_2.dll"

#define LOAD_LIBRARY_SEARCH_SYSTEM32			0x00000800
#define LOAD_LIBRARY_SEARCH_APPLICATION_DIR		0x00000200

namespace proxy
{
    void init();
    void shutdown();
}