#include "watch_clipboard.h"


LRESULT CALLBACK ClipboardViewerCallback(HWND hWnd, UINT msg, WPARAM wParam,
                                         LPARAM lParam) {
  switch (msg) {
  case WM_DRAWCLIPBOARD:
    std::cout << "Clipboard content has changed." << std::endl;
    read_clipboard();

    break;
  case WM_CHANGECBCHAIN:
    if (reinterpret_cast<HWND>(wParam) == nextClipboardViewer)
      nextClipboardViewer = reinterpret_cast<HWND>(lParam);
    else if (nextClipboardViewer != NULL)
      SendMessage(nextClipboardViewer, msg, wParam, lParam);
    break;
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
int main() {
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = 0;
  wcex.lpfnWndProc = ClipboardViewerCallback;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = GetModuleHandle(NULL);
  wcex.hIcon = NULL;
  wcex.hCursor = NULL;
  wcex.hbrBackground = NULL;
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = ("MyMessageOnlyWindow");
  wcex.hIconSm = NULL;

  if (!RegisterClassEx(&wcex)) {
    std::cerr << "Failed to register window class." << std::endl;
    return 1;
  }

  HWND hWnd = CreateWindowEx(0, ("MyMessageOnlyWindow"), (""), 0, 0, 0, 0, 0,
                             HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
  if (hWnd == NULL) {
    std::cerr << "Failed to create message-only window." << std::endl;
    return 1;
  }

  nextClipboardViewer = SetClipboardViewer(hWnd);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  ChangeClipboardChain(hWnd, nextClipboardViewer);

  return 0;
}
int read_clipboard() {
  if (!OpenClipboard(NULL)) {
    std::cerr << "Failed to open clipboard." << std::endl;
    return 1;
  }

  HANDLE clipboardDataHandle = GetClipboardData(CF_TEXT);
  if (clipboardDataHandle == NULL) {
    std::cerr << "Failed to retrieve clipboard data." << std::endl;
    CloseClipboard();
    return 1;
  }

  char *clipboardText = static_cast<char *>(GlobalLock(clipboardDataHandle));
  if (clipboardText == NULL) {
    std::cerr << "Failed to lock clipboard data." << std::endl;
    CloseClipboard();
    return 1;
  }

  std::cout << "Clipboard Text: " << clipboardText << std::endl;

  GlobalUnlock(clipboardDataHandle);
  CloseClipboard();
}
