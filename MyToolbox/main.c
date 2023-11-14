#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

#include "resource.h"
#include "globals.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define IDC_LISTTOOL 10001

static LPCWSTR g_appName = L"QianNangong's Toolbox";

typedef struct tagTOOLENTRY
{
	LPCWSTR toolName;
	SHORT dialogId;
	DLGPROC dialogProc;
} TOOLENTRY;

static
const TOOLENTRY g_entries[] =
{
	{ L"MD5 Calculator", IDD_MD5, MD5DialogProc },
	{ L"2", 0, NULL },
};

static
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		HWND list = CreateWindowExW(0, WC_LISTBOX, L"", WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_EXTENDEDSEL, 0, 0, 0, 0, hwnd, (HMENU)IDC_LISTTOOL, g_hinst, NULL);
		for (int i = 0; i < (sizeof(g_entries) / sizeof(TOOLENTRY)); i++)
		{
			SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)g_entries[i].toolName);
		}
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		MoveWindow(GetDlgItem(hwnd, IDC_LISTTOOL), 0, 0, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		return 0;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wparam))
		{
		case IDC_LISTTOOL:
		{
			switch (HIWORD(wparam))
			{
			case LBN_DBLCLK:
			{
				int sel = (int)SendDlgItemMessageW(hwnd, IDC_LISTTOOL, LB_GETCURSEL, 0, 0);
				if (g_entries[sel].dialogId == 0 || g_entries[sel].dialogProc == NULL)
				{
					WCHAR buf[128];
					wsprintf(buf, L"%s is not implemented.", g_entries[sel].toolName);
					MessageBoxW(hwnd, buf, g_appName, MB_OK | MB_ICONERROR);
				}
				else
				{
					ShowWindow(hwnd, SW_HIDE);
					DialogBoxParamW(g_hinst, MAKEINTRESOURCEW(g_entries[sel].dialogId), hwnd, g_entries[sel].dialogProc, NULL);
					ShowWindow(hwnd, SW_SHOW);
				}
				return 0;
			}
			}
			__fallthrough;
		}
		}
		__fallthrough;
	}
	default:
	{
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
	}
	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	(void)hPrevInstance;
	(void)lpCmdLine;

	WCHAR* const className = L"MyToolboxApp";
	WNDCLASSEXW wc =
	{
		.cbSize = sizeof(WNDCLASSEXW),
		.lpfnWndProc = MainWindowProc,
		.hInstance = hInstance,
		.lpszClassName = className,
	};
	if (!RegisterClassExW(&wc))
	{
		MessageBoxW(NULL, L"Failed to register window class.", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	HWND hwnd = CreateWindowExW(0, className, g_appName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		MessageBoxW(NULL, L"Failed to create window handle.", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return 0;
}