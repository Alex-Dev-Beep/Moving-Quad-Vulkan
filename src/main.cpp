#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "instance/instance.hpp"

HINSTANCE g_hInstance;
HWND g_hWnd;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND CreateVulkanWindow(uint32_t width, uint32_t height)
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

VkSurfaceKHR CreateSurface(VkInstance instance)
{
    VkWin32SurfaceCreateInfoKHR ci{};
    ci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    ci.hinstance = g_hInstance;
    ci.hwnd = g_hWnd;

    VkSurfaceKHR surface;
    if (vkCreateWin32SurfaceKHR(instance, &ci, nullptr, &surface) != VK_SUCCESS)
        throw std::runtime_error("Failed to create surface");

    return surface;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    #ifdef _DEBUG
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
    #endif

    g_hInstance = hInstance;

    g_hWnd = CreateVulkanWindow(800, 600);

    VkInstance instance = CreateVulkanInstance();
    VkSurfaceKHR surface = CreateSurface(instance);

    MSG msg{};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    return 0;
}
