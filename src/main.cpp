#define WIN32_LEAN_AND_MEAN
#define UNICODE
#define _UNICODE
#define VK_USE_PLATFORM_WIN32_KHR

#include <windows.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

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

VkInstance CreateVulkanInstance()
{
    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Win32 Vulkan App";
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ci.pApplicationInfo = &appInfo;
    ci.enabledExtensionCount = 2;
    ci.ppEnabledExtensionNames = extensions;

    VkInstance instance;
    if (vkCreateInstance(&ci, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create instance");

    return instance;
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
