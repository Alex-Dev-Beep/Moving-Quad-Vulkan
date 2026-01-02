#include <windows.h>
#include "vulkan/vulkan_common.h"
#include <vector>
#include <stdexcept>
#include <iostream>

#include "instance/instance.hpp"
#include "../renderer/window/hWnd.h"
#include "../renderer/surface/surface.h"

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
    
    if (createVulkanInstance(&instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    VkSurfaceKHR surface = CreateSurface(instance, g_hWnd, g_hInstance);

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
    destroyVulkanInstance(instance);
    return 0;
}
