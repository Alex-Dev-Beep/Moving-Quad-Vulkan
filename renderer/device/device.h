#include <vulkan/vulkan.h>
#include <optional>
void pickPhysicalDevice(VkInstance instance, VkPhysicalDevice& physicalDevice);
int rateDeviceSuitability(VkPhysicalDevice device);

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);