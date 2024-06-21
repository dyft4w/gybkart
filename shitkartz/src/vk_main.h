#pragma once
#include "includes.h"
#include "vk_structs.h"
#include "util.h"
#include <optional>
#include <vulkan/vulkan_core.h>

class VkEngine{
private:
  // do not touch
  VkInstance *m_vk_inst;
  SDL_Window *m_window;
  std::vector<VkPhysicalDevice> *m_physical_devices;
  std::vector<VkImage> *m_swapchain_images;
  std::vector<VkImageView> *m_image_views;
  VkDevice *m_vk_device;
  VkQueue *m_graphics_queue;
  VkQueue *m_present_queue;
  VkSwapchainKHR *m_vk_swapchain;
  VkFormat *m_swapchain_format;
  VkExtent2D *m_swapchain_extent;
  size_t *m_main_device_index;
  VkSurfaceKHR *m_vk_surface;
  
  // do touch
  const std::vector<const char *> m_device_extensions = {
    "VK_KHR_swapchain"
  };
  const std::vector<const char*> m_val_lay = {
    "VK_LAYER_KHRONOS_validation"
  };

  std::vector<VkDynamicState> m_dynamic_states = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };

  // prefs
  bool m_use_dedicated;
  const bool m_enable_val_lay = true;
public:
  VkEngine(SDL_Window *w, bool use_dedicated=false);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> *available_formats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> *available_presents);
  VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR *capabilities);
  std::optional<VkShaderModule> createShaderModules(const char* data, size_t size);

  bool checkLayerSupport();
  bool createInstance();
  bool checkDeviceExtentionSupport(VkPhysicalDevice *device);
  bool isDeviceSuitable(VkPhysicalDevice *device);
  bool pickPhysical();
  void cleanup();
  bool createLogical();
  bool createSurface();
  bool createSwapChain();
  bool createImageViews();
  bool createGraphicsPipeline();
  bool initVulkan();
};
