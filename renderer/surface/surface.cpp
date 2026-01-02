#include <windows.h>
#include "surface.h"
#include <stdexcept>
#include <iostream>

VkSurfaceKHR CreateSurface(VkInstance instance, HWND g_hWnd, HINSTANCE g_hInstance)
{
    std::cout << "Creating surface..." << std::endl;
    VkWin32SurfaceCreateInfoKHR ci{};
    ci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    ci.hinstance = g_hInstance;
    ci.hwnd = g_hWnd;

    VkSurfaceKHR surface;
    VkResult surfaceCreationResult = vkCreateWin32SurfaceKHR(instance, &ci, nullptr, &surface);
    if (surfaceCreationResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to create surface");
    } else {
        std::cout << "Created surface succesfully." << std::endl;
    }

    return surface;
}