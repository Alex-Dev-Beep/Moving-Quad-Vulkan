#include <vulkan/vulkan.h>
#include "../instance/instance.hpp"

void cleanup(VkInstance instance, VkSurfaceKHR surface) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
    destroyVulkanInstance(instance);
}