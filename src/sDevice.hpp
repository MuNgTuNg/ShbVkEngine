#pragma once
#include <vector>
#include "sWindow.hpp"
#include <cstring>
#include "sDebug.hpp"

namespace shb{

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif



struct QueueFamilyIndices{
  uint32_t graphicsFamily;             //index of queue family on device that supports graphical commands
  uint32_t presentFamily;              //index of queue family on device that supports presenting to screen
  bool gfxFamilyHasValue = false;      //boolean indicating that a value has been assigned to graphicsFamily index
  bool presentFamilyHasValue = false;  //boolean indicating that a value has been assigned to presentFamily index


  bool isComplete() {
    return gfxFamilyHasValue && presentFamilyHasValue;  //we have completed a search for queue family indeces
                                                        //as all indexes have been assigned to
  }


  int totalQueues =0 ;         //indicates the amount of queues added in in total
  std::vector<int> queueIndicesArray; 
};


class sWindow;

class sDevice{
 public:
  sDevice(sWindow& window) : _window(window){ 
    initVulkan();
  }
  ~sDevice();

  VkQueue getGraphicsQueue() const { return _graphicsQueue;  }
  VkDevice getDevice() const { return _device; }
  VkSurfaceKHR getSurface() const { return _surface; }
  VkPhysicalDevice getPhysicalDevice() const { return _gpu ; }
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  
 private:
  void setupDebugMessenger();
  void createInstance();
  void initVulkan();
  void pickPhysicalDevice();
  void createLogicalDevice();
  bool checkValidationLayerSupport();


  bool isDeviceSuitable(VkPhysicalDevice& device);

  
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