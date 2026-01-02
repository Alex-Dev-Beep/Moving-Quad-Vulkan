#include <vulkan/vulkan.h>
#include <stdexcept>
#include <iostream>

VkInstance CreateVulkanInstance()
{

    std::cout << "Creating instance..." << std::endl;

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
    VkResult createInstanceResult = vkCreateInstance(&ci, nullptr, &instance);
    if (createInstanceResult != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance");
    } else {
        std::cout << "Created instance succesfully." << std::endl;
    }

    return instance;
}