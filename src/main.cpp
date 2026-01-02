#include <windows.h>
#include "vulkan/vulkan_common.h"
#include <vector>
#include <stdexcept>
#include <iostream>

#include "instance/instance.hpp"
#include "../renderer/window/hWnd.h"
#include "../renderer/surface/surface.h"
#include "cleanup/cleanup.h"

HINSTANCE g_hInstance;

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
    
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkSurfaceKHR surface = CreateSurface(instance, g_hWnd, g_hInstance);
    
    if (createVulkanInstance(&instance) != VK_SUCCESS) {
        std::cout << "Failed to create vulkan instance" << std::endl;
    }

    MSG msg{};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    cleanup(instance, surface);
    return 0;
}
