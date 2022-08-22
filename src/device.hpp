#pragma once
#include <vector>
#include <iostream>
#include "shb_debug.hpp"

namespace shb{


class Device{
 public:   
    Device();
    ~Device();
    


 private:
    void cleanup();

    void initVulkan();
    void createInstance();
    void setupDebugMessenger();
    void createPhysicalDevice();
    void createLogicalDevice();

    VkPhysicalDevice _physicalDevice;
    VkQueueFamilyProperties _queueFamProps;

    VkDevice _device;
    VkInstance _instance;
    ShbDebug* _debug = new ShbDebug(_instance);
    
    std::vector<const char*> _extensions{};
    
};

}//namespace shb
