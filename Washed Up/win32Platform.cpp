#include <windows.h>
#include <string>

bool running = true;

void *buffer_memory;
int buffer_width;
int buffer_height;
BITMAPINFO buffer_bitmap_info;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam,
                                 LPARAM lParam) {
  LRESULT result = 0;

  switch (uMsg) {
  case WM_CLOSE: {
    running = false;
    break;
  }
  case WM_DESTROY: {
    running = false;
  } break;
  case WM_SIZE: {
    RECT rect;
    GetClientRect(hwnd, &rect);

    buffer_width = rect.right - rect.left;
    buffer_height = rect.bottom - rect.top;

    int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);

    if (buffer_memory)
      VirtualFree(buffer_memory, 0, MEM_RELEASE);
    buffer_memory =
        VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
    buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
    buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
    buffer_bitmap_info.bmiHeader.biPlanes = 1;
    buffer_bitmap_info.bmiHeader.biBitCount = 32;
    buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
  } break;
  default:
    result = DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  return result;
}

inline std::string charToStr(char charac) {
  std::string str;
  str.push_back(charac);
  return str;
}

#include "renderer.cpp"
#include "washedUp.cpp"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
            int nShowCmd) {
  const wchar_t class_name[] = L"Washed Up";
  WNDCLASS window_class = {};
  window_class.style = CS_HREDRAW | CS_VREDRAW;
  window_class.hInstance = hInstance;
  window_class.lpszClassName = class_name;
  window_class.lpfnWndProc = window_callback;

  RegisterClass(&window_class);
  /* HWND window = CreateWindowExW(
      0, class_name, window_class.lpszClassName, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, GetSystemMetrics(SM_CXSCREEN),
      GetSystemMetrics(SM_CXSCREEN), NULL, NULL, hInstance, NULL);
  ShowWindow(window, nShowCmd);*/
  HWND window = CreateWindowExW(
      0, class_name, window_class.lpszClassName, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  ShowWindow(window, nShowCmd);
  HDC hdc = GetDC(window);

  washedUp(window);
}