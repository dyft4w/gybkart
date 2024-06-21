#include "vk_structs.h"
bool QueueFamilyIndices::isComplete(){
    return graphics_family.has_value() && present_family.has_value();
}
bool QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR *vk_surface){
    uint32_t queue_family_count=0;
    VkBool32 present_support;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    for(size_t i=0; i < queue_families.size();++i){
      if(queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
        graphics_family = i;
        present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *vk_surface, &present_support);
        if(present_support){
          present_family=i;
        }
      }
    }


    return true;
  }
  bool SwapChainSupportDetails::querySwapChainSupport(VkPhysicalDevice *device, VkSurfaceKHR *surface){
    uint32_t format_count;
    uint32_t modes_count;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, *surface, &caps);
    vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &format_count, nullptr);
    vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &modes_count, nullptr);

    if(format_count == 0 || modes_count == 0){
      return false;
    }

    formats = new std::vector<VkSurfaceFormatKHR>(format_count);
    present_modes = new std::vector<VkPresentModeKHR>(modes_count);

    vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &format_count, formats->data());
    vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &modes_count, present_modes->data());

    return true;
  }
