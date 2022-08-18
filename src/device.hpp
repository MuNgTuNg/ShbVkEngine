#pragma once
#include <iostream>
#include "shb_debug.hpp"
namespace shb{


// const bool enableValidationLayers = true;

// const std::vector<const char*> validationLayers = {
//     "VK_LAYER_KHRONOS_validation"
// };

class Device{
 public:   
    Device();
    ~Device();
    


 private:
    void cleanup();

    void initVulkan();
    void createInstance();
    void setupDebugMessenger();

    VkDevice _device;
    VkInstance _instance;
    ShbDebug* _debug = new ShbDebug(_instance);
    //ShbDebug* _debug2 = new ShbDebug(_instance);
    
    
};

}//namespace shb