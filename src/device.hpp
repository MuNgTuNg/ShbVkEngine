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
  int graphicsFamily;
  int presentFamily;
  bool gfxFamilyHasValue = false;
  bool presentFamilyHasValue = false;

  bool isComplete() {
    return gfxFamilyHasValue && presentFamilyHasValue;
  }
};

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
  

  bool isDeviceSuitable(VkPhysicalDevice& device);

 private:
  
  sWindow& _window;
  VkInstance _instance;
  VkPhysicalDevice _gpu;
  VkDevice _device;
  VkSurfaceKHR _surface;

  VkDebugUtilsMessengerEXT debugMessenger;

  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  VkQueue _graphicsQueue;
  


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};




  

};

}//namespace shb