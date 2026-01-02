#include <vulkan/vulkan.h>

void pickPhysicalDevice(VkInstance instance, VkPhysicalDevice& physicalDevice);
int rateDeviceSuitability(VkPhysicalDevice device);