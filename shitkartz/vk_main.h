#pragma once
#include "SDL_video.h"
#include "SDL_vulkan.h"
#include "includes.h"
#include <cstddef>
#include <vulkan/vulkan_core.h>
struct QueueFamilyIndices{
  std::optional<uint32_t> graphics_family;
  std::optional<uint32_t> present_family;

  bool isComplete(){
    return graphics_family.has_value() && present_family.has_value();
  }
};
class VkEngine{
private:
  VkInstance *vk_inst;
  SDL_Window *window;
  std::vector<VkPhysicalDevice> *physical_devices;
  VkDevice *vk_device;
  VkQueue *graphics_queue;
  VkQueue *present_queue;
  size_t *main_device_index;
  VkSurfaceKHR *vk_surface;
  // prefs
  bool _use_dedicated;
  //VkApplicationInfo *appInfo;
public:
  VkEngine(SDL_Window *w, bool use_dedicated=false){
    _use_dedicated = use_dedicated;
    window = w;
    initVulkan(w);
  }

  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice *device){
    QueueFamilyIndices indices;
    uint32_t queue_family_count=0;
    VkBool32 present_support;

    vkGetPhysicalDeviceQueueFamilyProperties(*device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    
    vkGetPhysicalDeviceQueueFamilyProperties(*device, &queue_family_count, queue_families.data());

    for(size_t i=0; i < queue_families.size();++i){
      if(queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
        indices.graphics_family = i;
        present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(*device, i, *vk_surface, &present_support);
        if(present_support){
          indices.present_family=i;
        }
      }
    }


    return indices;
  }

  bool createInstance(){
    vk_inst = new VkInstance;
    VkApplicationInfo appInfo;
    VkInstanceCreateInfo create_info;
    const char **extensions = 0;
    uint32_t extensions_count;
    SDL_Vulkan_GetInstanceExtensions(window, &extensions_count, nullptr);
    extensions = new const char *[extensions_count];
    SDL_Vulkan_GetInstanceExtensions(window, &extensions_count, extensions);
    //extensions = new const char *[extensions_count];

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = SDL_GetWindowTitle(window);
    appInfo.applicationVersion = VK_MAKE_VERSION(0,8,0);
    appInfo.pEngineName = "real engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &appInfo;
    create_info.enabledExtensionCount = extensions_count;
    create_info.ppEnabledExtensionNames = extensions;
    create_info.enabledLayerCount = 0;

    if(vkCreateInstance(&create_info, nullptr, vk_inst) != VK_SUCCESS){
      return false;
    }
    return true;

  }
  bool checkDeviceExtentionSupport(VkPhysicalDevice *device){
    uint32_t extension_count;
    std::vector<VkExtensionProperties> *avail_ext;

    vkEnumerateDeviceExtensionProperties(*device, nullptr, &extension_count, nullptr);
    avail_ext = new std::vector<VkExtensionProperties>(extension_count);
    vkEnumerateDeviceExtensionProperties(*device, nullptr, &extension_count, avail_ext->data());

    std::set<std::string> required_extensions(avail_ext->begin(),avail_ext->end());

    return true;
  }
  bool isDeviceSuitable(VkPhysicalDevice *device){
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;
    QueueFamilyIndices indices = findQueueFamilies(device);

    vkGetPhysicalDeviceProperties(*device, &device_properties);
    vkGetPhysicalDeviceFeatures(*device, &device_features);

    // ranking errors from most important to least
    if(!indices.isComplete()){
      return false;
    }
    if(!checkDeviceExtentionSupport(device)){
      return false;
    }
    if(_use_dedicated && !(device_properties.deviceType==VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)){
      return false;
    }
    return true;
  }
  bool pickPhysical(){
    uint32_t device_count = 0;
    main_device_index = new size_t(-1);
    vkEnumeratePhysicalDevices(*vk_inst, &device_count, nullptr);

    if(device_count==0){
      std::cerr << "No vulkan devices found!\n"; 
      return false;
    }
    physical_devices = new std::vector<VkPhysicalDevice>(device_count);

    vkEnumeratePhysicalDevices(*vk_inst, &device_count, physical_devices->data());
    
    for(size_t i = 0;i<physical_devices->size();++i){
      if(isDeviceSuitable(&physical_devices->at(i))){
        *main_device_index=i;
        break;
      }
    }
    if(physical_devices->at(*main_device_index)==VK_NULL_HANDLE){
      std::cerr << "No suitable GPU found!\n";
      return false;
    }

    
    return true;
  }
  void cleanup(){
    vkDestroyDevice(*vk_device, nullptr);
    vkDestroySurfaceKHR(*vk_inst, *vk_surface, nullptr);
    vkDestroyInstance(*vk_inst, nullptr);
    delete vk_device;
    delete vk_surface;
    delete vk_inst;
  }
  bool createLogical(){
    QueueFamilyIndices indices = findQueueFamilies(&physical_devices->at(*main_device_index));
    VkPhysicalDeviceFeatures phy_features;
    std::set<uint32_t> unique_queue_families = {indices.graphics_family.value(),indices.present_family.value()};
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    graphics_queue = new VkQueue(); 
    present_queue = new VkQueue(); 
    vk_device = new VkDevice();

    float queuePriority = 1.0f;
    for(uint32_t queue_family : unique_queue_families){
      VkDeviceQueueCreateInfo queue_create_info;
      queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_create_info.queueFamilyIndex = queue_family;
      queue_create_info.queueCount = 1;
      queue_create_info.pQueuePriorities = &queuePriority;
      queue_create_infos.push_back(queue_create_info);
    }
    vkGetPhysicalDeviceFeatures(physical_devices->at(*main_device_index), &phy_features);
    
    VkDeviceCreateInfo create_info;
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.queueCreateInfoCount = (uint32_t)(queue_create_infos.size());
    create_info.pEnabledFeatures = &phy_features;
    
    if(!vkCreateDevice(physical_devices->at(*main_device_index), &create_info, nullptr, vk_device)){
      std::cerr << "Failed to create vulkan device\n";
      return false;
    }
    vkGetDeviceQueue(*vk_device, indices.graphics_family.value(), 0, graphics_queue);
    vkGetDeviceQueue(*vk_device, indices.present_family.value(), 0, present_queue);
    return true;
  }
  bool createSurface(SDL_Window *w){
    vk_surface = new VkSurfaceKHR();
    if(SDL_Vulkan_CreateSurface(w, *vk_inst, vk_surface)==SDL_FALSE){
      return false;
    }

    return true;
  }
  bool initVulkan(SDL_Window *w){
    if(!createInstance() || !pickPhysical() || !createLogical() || createSurface(w)){
      return false;
    }
    return true;
  }
};
