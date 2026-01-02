#include "../vulkan/vulkan_common.h"

#include <vector>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "instance.hpp"

#ifndef NDEBUG
static const bool enableValidationLayers = true;
#else
static const bool enableValidationLayers = false;
#endif

static const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static VkDebugUtilsMessengerEXT debugMessenger;

/* ---------------------------------------------------------- */
/* Validation layer support                                   */
/* ---------------------------------------------------------- */

bool checkValidationLayerSupport() {
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> layers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    for (const char* name : validationLayers) {
        bool found = false;
        for (const auto& layer : layers) {
            if (strcmp(name, layer.layerName) == 0) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    return true;
}

/* ---------------------------------------------------------- */
/* Debug callback                                             */
/* ---------------------------------------------------------- */

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void*
) {
    if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        std::cerr << "[Validation] " << data->pMessage << std::endl;
    }
    return VK_FALSE;
}

/* ---------------------------------------------------------- */
/* Debug messenger helpers                                    */
/* ---------------------------------------------------------- */

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* info,
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT* messenger
) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    return func ? func(instance, info, allocator, messenger)
                : VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* allocator
) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func) func(instance, messenger, allocator);
}

/* ---------------------------------------------------------- */
/* Instance creation                                          */
/* ---------------------------------------------------------- */

VkResult createVulkanInstance(VkInstance* instance) {
    if (!instance) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested but not available");
    }

    /* ---- Extensions ---- */

    std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    /* ---- App info ---- */

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Sandbox";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Custom Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    /* ---- Debug messenger info (pNext) ---- */

    VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
    if (enableValidationLayers) {
        debugInfo.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugInfo.pfnUserCallback = debugCallback;
    }

    /* ---- Instance create info ---- */

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        createInfo.pNext = &debugInfo;
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, instance);
    if (result != VK_SUCCESS) {
        return result;
    }

    /* ---- Create debug messenger AFTER instance ---- */

    if (enableValidationLayers) {
        CreateDebugUtilsMessengerEXT(
            *instance, &debugInfo, nullptr, &debugMessenger);
    }

    std::cout << "Vulkan instance created successfully\n";
    return VK_SUCCESS;
}

void destroyVulkanInstance(VkInstance instance) {
    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    vkDestroyInstance(instance, nullptr);
}
