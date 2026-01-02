#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "vulkan/vulkan_common.h"
#include <vector>
#include <stdexcept>
#include <iostream>

#include "instance/instance.hpp"
#include "../renderer/window/hWnd.h"

HINSTANCE g_hInstance;

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

    g_hWnd = CreateVulkanWindow(800, 600, g_hInstance);

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
