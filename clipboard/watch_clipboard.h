#include <iostream>
#include <Windows.h>

HWND nextClipboardViewer = NULL;

int read_clipboard();
LRESULT CALLBACK ClipboardViewerCallback(HWND hWnd, UINT msg, WPARAM wParam,
                                         LPARAM lParam);
