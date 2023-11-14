#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern IMAGE_DOS_HEADER __ImageBase;

#define g_hinst ((HINSTANCE)&__ImageBase)

extern INT_PTR MD5DialogProc(HWND, UINT, WPARAM, LPARAM);


#ifdef __cplusplus
}
#endif // __cplusplus
