#include <vulkan/vulkan.h>
#include "../instance/instance.hpp"

void cleanup(VkInstance instance, VkSurfaceKHR surface, VkDevice device) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
    destroyVulkanInstance(instance);
    vkDestroyDevice(device, nullptr);
}