#pragma once
#include <string>
#include <vulkan/vulkan.hpp>
#define VK_USE_PLATFORM_XCB
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <iostream>
#include <vector> 
#include <cstring>

#include "shb_debug.hpp"
#include "window.hpp"
namespace shb{

struct QueueFamilyIndices {
  uint32_t graphicsFamily;
  uint32_t presentFamily;
  bool graphicsFamilyHasValue;
  bool presentFamilyHasValue;
  
  bool isComplete() { return graphicsFamilyHasValue; /*&& presentFamilyHasValue;*/ } //essentially searching for all queues specified, 
                                                                                     //represented by indeces
};
 

const bool enableValidationLayers = true;


class Device{
 public:   
    Device() = delete;
    Device(Window* w) : _window(w) {
      initVulkan();
    }
    ~Device();
    


 private:
    void cleanup();

    void initVulkan();
    void createInstance();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSurface();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);
    bool isDeviceSuitable(VkPhysicalDevice);
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
    bool checkValidationLayerSupport();
    std::vector<const char *> getRequiredExtensions();


    VkPhysicalDevice _physicalDevice{}; //= VK_NULL_HANDLE;
    VkQueueFamilyProperties _queueFamProps;

    VkDevice _device;
    VkInstance _instance;
    ShbDebug* _debug = new ShbDebug();
    VkQueue graphicsQueue;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR _surface;

    Window* _window;
    std::vector<const char*> _extensions{};
    
    static const int NUM_QUEUES = 2;
    VkDeviceQueueCreateInfo deviceQueues[NUM_QUEUES];

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    
};

}//namespace shb
