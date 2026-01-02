#pragma once
#include <windows.h>
#include "../../src/vulkan/vulkan_common.h"

extern HWND g_hWnd;

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

HWND CreateVulkanWindow(
    uint32_t width,
    uint32_t height,
    HINSTANCE hInstance
);
