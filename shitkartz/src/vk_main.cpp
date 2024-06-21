#include "vk_main.h"
VkEngine::VkEngine(SDL_Window *w, bool use_dedicated){
  m_use_dedicated = use_dedicated;
  m_window = w;
  initVulkan();
}

VkSurfaceFormatKHR VkEngine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> *available_formats) {
  for(const auto& format : *available_formats){
    if(format.format == VK_FORMAT_B8G8R8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
      return format;
    }
  } 
  return (*available_formats)[0];
}
VkPresentModeKHR VkEngine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> *available_presents){
  if(std::find(available_presents->begin(),available_presents->end(),VK_PRESENT_MODE_IMMEDIATE_KHR) != std::end(*available_presents)){
    return VK_PRESENT_MODE_IMMEDIATE_KHR;
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VkEngine::chooseExtent(const VkSurfaceCapabilitiesKHR *capabilities){
  if(capabilities->currentExtent.width != std::numeric_limits<uint32_t>::max()){
    return capabilities->currentExtent;
  }
  int w,h;
  SDL_Vulkan_GetDrawableSize(m_window, &w, &h);
  
  VkExtent2D extent{
    (uint32_t) w,
    (uint32_t) h
  };
  extent.width=std::clamp(extent.width,capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
  extent.height=std::clamp(extent.height,capabilities->minImageExtent.height, capabilities->maxImageExtent.height);


  return extent;
}
std::optional<VkShaderModule> VkEngine::createShaderModules(const char* data, size_t size){
  VkShaderModuleCreateInfo create_info{};
  VkShaderModule shader_module;
  
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = size;
  create_info.pCode = (const uint32_t*) data;
  if(vkCreateShaderModule(*m_vk_device, &create_info, nullptr, &shader_module) != VK_SUCCESS){
    std::cerr << "failed to create shader module\n";
    return std::nullopt;
  }
  return shader_module;
}
bool VkEngine::checkLayerSupport(){
  uint32_t layer_count;
  std::vector<VkLayerProperties> avail_layers;
  std::vector<const char *> str_layers;

  vkEnumerateInstanceLayerProperties(&layer_count, nullptr); 
  avail_layers.resize(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, avail_layers.data()); 
  for(const auto& i: avail_layers){
    str_layers.push_back(i.layerName);
  }

  for(const auto& i : m_val_lay){
    if(std::find(str_layers.begin(),str_layers.end(),i)==str_layers.end()){
      return false;
    }
  }
  return true;
}

bool VkEngine::createInstance(){
  m_vk_inst = new VkInstance;
  VkApplicationInfo appInfo{};
  VkInstanceCreateInfo create_info{};
  const char **extensions = 0;
  uint32_t extensions_count;
  SDL_Vulkan_GetInstanceExtensions(m_window, &extensions_count, nullptr);
  extensions = new const char *[extensions_count];
  SDL_Vulkan_GetInstanceExtensions(m_window, &extensions_count, extensions);
  //extensions = new const char *[extensions_count];

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = SDL_GetWindowTitle(m_window);
  appInfo.applicationVersion = VK_MAKE_VERSION(0,8,0);
  appInfo.pEngineName = "real engine";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &appInfo;
  create_info.enabledExtensionCount = extensions_count;
  create_info.ppEnabledExtensionNames = extensions;
  create_info.enabledLayerCount = 0;

  if(vkCreateInstance(&create_info, nullptr, m_vk_inst) != VK_SUCCESS){
    return false;
  }
  return true;

}
bool VkEngine::checkDeviceExtentionSupport(VkPhysicalDevice *device){
  uint32_t extension_count;
  std::vector<VkExtensionProperties> *avail_ext;

  vkEnumerateDeviceExtensionProperties(*device, nullptr, &extension_count, nullptr);
  avail_ext = new std::vector<VkExtensionProperties>(extension_count);
  vkEnumerateDeviceExtensionProperties(*device, nullptr, &extension_count, avail_ext->data());

  std::set<std::string> required_extensions(m_device_extensions.begin(),m_device_extensions.end());

  for(const auto& extension : *avail_ext){
    required_extensions.erase(extension.extensionName); 
  }
  return required_extensions.empty();
}
bool VkEngine::isDeviceSuitable(VkPhysicalDevice *device){
  VkPhysicalDeviceProperties device_properties{};
  VkPhysicalDeviceFeatures device_features{};
  SwapChainSupportDetails swap{};
  QueueFamilyIndices indices{};
  indices.findQueueFamilies(*device,m_vk_surface);

  swap.querySwapChainSupport(device, m_vk_surface);
  vkGetPhysicalDeviceProperties(*device, &device_properties);
  vkGetPhysicalDeviceFeatures(*device, &device_features);

  // ranking errors from most important to least
  if(!indices.isComplete()){
    return false;
  }
  if(!checkDeviceExtentionSupport(device)){
    return false;
  }
  if(swap.formats->empty() || swap.present_modes->empty()){
    return false;
  }
  if(m_use_dedicated && !(device_properties.deviceType==VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)){
    return false;
  }
  return true;
}
bool VkEngine::pickPhysical(){
  uint32_t device_count = 0;
  m_main_device_index = new size_t(-1);

  vkEnumeratePhysicalDevices(*m_vk_inst, &device_count, nullptr);

  if(device_count==0){
    std::cerr << "No vulkan devices found!\n"; 
    return false;
  }

  m_physical_devices = new std::vector<VkPhysicalDevice>(device_count);
  vkEnumeratePhysicalDevices(*m_vk_inst, &device_count, m_physical_devices->data());
  
  for(size_t i = 0;i<m_physical_devices->size();++i){
    if(isDeviceSuitable(&(*m_physical_devices)[i])){
      *m_main_device_index=i;
      break;
    }
  }
  if((*m_physical_devices)[*m_main_device_index]==VK_NULL_HANDLE){
    std::cerr << "No suitable GPU found!\n";
    return false;
  }
  return true;
}
void VkEngine::cleanup(){
  for(auto i : *m_image_views){
    vkDestroyImageView(*m_vk_device, i, nullptr);
  }
  vkDestroySwapchainKHR(*m_vk_device, *m_vk_swapchain, nullptr);
  vkDestroyDevice(*m_vk_device, nullptr);
  vkDestroySurfaceKHR(*m_vk_inst, *m_vk_surface, nullptr);
  vkDestroyInstance(*m_vk_inst, nullptr);
  delete m_image_views;
  delete m_vk_swapchain;
  delete m_vk_device;
  delete m_vk_surface;
  delete m_vk_inst;
}
bool VkEngine::createLogical(){
  QueueFamilyIndices indices{};
  indices.findQueueFamilies(m_physical_devices->at(*m_main_device_index), m_vk_surface);
  VkPhysicalDeviceFeatures phy_features{};
  std::set<uint32_t> unique_queue_families = {indices.graphics_family.value(),indices.present_family.value()};
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  m_graphics_queue = new VkQueue(); 
  m_present_queue = new VkQueue(); 
  m_vk_device = new VkDevice();
  float queuePriority = 1.0f;

  for(uint32_t queue_family : unique_queue_families){
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queuePriority;
    queue_create_infos.push_back(queue_create_info);
  }
  vkGetPhysicalDeviceFeatures((*m_physical_devices)[*m_main_device_index], &phy_features);
  
  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()); //(uint32_t)(queue_create_infos.size());
  create_info.pEnabledFeatures = &phy_features;
  create_info.enabledExtensionCount = static_cast<uint32_t>(m_device_extensions.size()); //(uint32_t) (device_extensions.size());
  create_info.ppEnabledExtensionNames = m_device_extensions.data();
  create_info.pNext = VK_NULL_HANDLE;
  create_info.enabledLayerCount = 0;
  create_info.ppEnabledLayerNames = VK_NULL_HANDLE;
  if(m_enable_val_lay){
    create_info.enabledLayerCount = static_cast<uint32_t>(m_val_lay.size());
    create_info.ppEnabledLayerNames = m_val_lay.data();
  }

  if(vkCreateDevice((*m_physical_devices)[*m_main_device_index], &create_info, nullptr, m_vk_device) != VK_SUCCESS){
    std::cerr << "Failed to create vulkan device\n";
    return false;
  }
  vkGetDeviceQueue(*m_vk_device, indices.graphics_family.value(), 0, m_graphics_queue);
  vkGetDeviceQueue(*m_vk_device, indices.present_family.value(), 0, m_present_queue);
  return true;
}
bool VkEngine::createSurface(){
  m_vk_surface = new VkSurfaceKHR();
  if(SDL_Vulkan_CreateSurface(m_window, *m_vk_inst, m_vk_surface)==SDL_FALSE){
    return false;
  }

  return true;
}
bool VkEngine::createSwapChain(){
  SwapChainSupportDetails swap;
  swap.querySwapChainSupport(&(*m_physical_devices)[*m_main_device_index], m_vk_surface);
  VkPresentModeKHR present_mode = chooseSwapPresentMode(swap.present_modes);
  VkSurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swap.formats);
  m_vk_swapchain = new VkSwapchainKHR();
  m_swapchain_extent = new VkExtent2D();
  m_swapchain_format = new VkFormat();
  QueueFamilyIndices indices;
  indices.findQueueFamilies((*m_physical_devices)[*m_main_device_index], m_vk_surface);
  uint32_t uint32_indices[] = {indices.graphics_family.value(), indices.present_family.value()};
  uint32_t chain_size = swap.caps.minImageCount+1;
  uint32_t image_count;

  if(chain_size>swap.caps.maxImageCount && swap.caps.maxImageCount>0){
    chain_size = swap.caps.maxImageCount;
  }

  *m_swapchain_extent = chooseExtent(&swap.caps);
  *m_swapchain_format = surface_format.format;

  VkSwapchainCreateInfoKHR create_info;
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = *m_vk_surface;
  create_info.minImageCount = chain_size;
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent = *m_swapchain_extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  if(indices.present_family != indices.graphics_family){
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
  }else{
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }
  create_info.queueFamilyIndexCount = 2;
  create_info.pQueueFamilyIndices = uint32_indices;
  create_info.preTransform = swap.caps.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = VK_NULL_HANDLE;
  
  if(vkCreateSwapchainKHR(*m_vk_device, &create_info, nullptr, m_vk_swapchain)!=VK_SUCCESS){
    std::cerr << "Failed to create swapchain\n";
    return false;
  };
  vkGetSwapchainImagesKHR(*m_vk_device, *m_vk_swapchain, &image_count, nullptr);
  m_swapchain_images = new std::vector<VkImage>(image_count);
  vkGetSwapchainImagesKHR(*m_vk_device, *m_vk_swapchain, &image_count, m_swapchain_images->data());

  return true;
}
bool VkEngine::createImageViews(){
  m_image_views = new std::vector<VkImageView>();
  for(auto &i : *m_swapchain_images){
    VkImageViewCreateInfo create_info;
    VkImageView iv;

    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = i;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    create_info.format = *m_swapchain_format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.layerCount = 1;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.baseArrayLayer = 0;

    if(vkCreateImageView(*m_vk_device, &create_info, nullptr, &iv)!=VK_SUCCESS){
      std::cerr << "could not create image views\n";
      return false;
    }
    m_image_views->push_back(iv);
  }
  return true;
}
bool VkEngine::createGraphicsPipeline(){
  // VkPipelineShaderStageCreateInfo vert_create_info{};
  // VkPipelineShaderStageCreateInfo frag_create_info{};
  VkPipelineShaderStageCreateInfo shader_stage_create_info[2]{};
  VkPipelineDynamicStateCreateInfo dynamic_create_info{};
  VkPipelineVertexInputStateCreateInfo vertex_input_create_info{};
  auto psv_vert = util::readFile("vert.psv");
  auto psv_frag = util::readFile("frag.psv");



  if(!psv_vert.has_value() || !psv_frag.has_value()){
    std::cerr << "failed to read shaders\n";
    return false;
  }
  auto shader_vert_opt = createShaderModules(psv_vert->data(), psv_vert->size());
  auto shader_frag_opt = createShaderModules(psv_frag->data(), psv_frag->size());
  if(!shader_frag_opt.has_value() || !shader_vert_opt.has_value()){
    std::cerr << "failed to convert shaders\n";
    return false;
  }
  

  shader_stage_create_info[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shader_stage_create_info[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  shader_stage_create_info[0].module = shader_vert_opt.value();
  shader_stage_create_info[0].pName = "main";
  
  shader_stage_create_info[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shader_stage_create_info[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  shader_stage_create_info[1].module = shader_frag_opt.value();
  shader_stage_create_info[1].pName = "main";

  dynamic_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_create_info.dynamicStateCount = m_dynamic_states.size();
  dynamic_create_info.pDynamicStates = m_dynamic_states.data();

  vertex_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_create_info.vertexBindingDescriptionCount = 0;
  vertex_input_create_info.pVertexBindingDescriptions = nullptr;
  vertex_input_create_info.vertexAttributeDescriptionCount = 0;
  vertex_input_create_info.pVertexAttributeDescriptions = nullptr;

  vkDestroyShaderModule(*m_vk_device, shader_vert_opt.value(), nullptr);
  vkDestroyShaderModule(*m_vk_device, shader_frag_opt.value(), nullptr);
  return true;
}
bool VkEngine::initVulkan(){
  if(m_enable_val_lay || !checkLayerSupport()){
    std::cerr << "avail lay\n";
  }
  if(!createInstance() || !createSurface() || !pickPhysical() || !createLogical() || !createSwapChain() || !createImageViews() || !createGraphicsPipeline()){
    return false;
  }
  return true;
}
