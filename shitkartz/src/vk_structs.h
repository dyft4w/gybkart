#pragma once
#include "includes.h"
struct QueueFamilyIndices{
  std::optional<uint32_t> graphics_family;
  std::optional<uint32_t> present_family;

  bool isComplete();
  bool findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR *vk_surface);
};

struct SwapChainSupportDetails{
  VkSurfaceCapabilitiesKHR caps;
  std::vector<VkSurfaceFormatKHR> *formats;
  std::vector<VkPresentModeKHR> *present_modes;
  bool querySwapChainSupport(VkPhysicalDevice *device, VkSurfaceKHR *surface);
};
