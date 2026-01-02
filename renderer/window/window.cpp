#include <windows.h>
#include "vulkan/vulkan_common.h"
#include "hWnd.h"

HWND CreateVulkanWindow(uint32_t width, uint32_t height, HINSTANCE g_hInstance)
{
    const wchar_t* CLASS_NAME = L"VulkanWindowClass";

    WNDCLASSW wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = g_hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Vulkan Win32 Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        g_hInstance,
        nullptr
    );

    ShowWindow(hwnd, SW_SHOW);
    return hwnd;
}