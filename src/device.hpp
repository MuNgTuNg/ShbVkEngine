#pragma once
#include <vector>
#include "window.hpp"
#include <cstring>
#include "shb_debug.hpp"

namespace shb{

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif



struct QueueFamilyIndices{
  uint32_t graphicsFamily;
  uint32_t presentFamily;
  bool gfxFamilyHasValue = false;
  bool presentFamilyHasValue = false;

  bool isComplete() {
    return gfxFamilyHasValue && presentFamilyHasValue;
  }

  int totalQueues =0 ;
  std::vector<int> queueArray;
};
class sWindow;

class sDevice{
 public:
  sDevice(sWindow& window) : _window(window){ 
    
    initVulkan();
    }
    
  
  ~sDevice();
  
  void setupDebugMessenger();
  void createInstance();
  void initVulkan();
  void pickPhysicalDevice();
  void createLogicalDevice();
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  bool isDeviceSuitable(VkPhysicalDevice& device);

  VkDevice getDevice() const { return _device; }
  VkSurfaceKHR getSurface() const { return _surface; }
  VkPhysicalDevice getPhysicalDevice() const { return _gpu ; }

  
  
  
 private:
  
  sWindow& _window;
  VkInstance _instance;
  VkPhysicalDevice _gpu;
  VkDevice _device;
  VkSurfaceKHR _surface;

  VkDebugUtilsMessengerEXT debugMessenger;

  VkQueue _graphicsQueue;
  
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};




  

};

}//namespace shb