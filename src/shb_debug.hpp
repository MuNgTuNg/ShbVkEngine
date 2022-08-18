#pragma once
#include <string>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector> 
#include <cstring>



namespace shb{
//need to make more interesting
    
const bool enableValidationLayers = false;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};


class ShbDebug{
 public:
    const bool DEBUG = false;
    const bool LogToFile = false;
    bool init = false;

    ShbDebug(VkInstance& instance) : _instance(instance) {}
    ShbDebug(VkInstance& instance, bool debug) : _instance(instance), DEBUG(debug) {}

    ~ShbDebug() {
        if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(_instance, debugMessenger, nullptr);
        }
    }


    std::vector<const char *> getRequiredExtensions();
    void setupDebugMessenger(VkInstance& instance);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool checkValidationLayerSupport();


    void log(const std::string& msg){
        if(DEBUG == true){
            std::cout<<msg;
        }
    }

    
    
    VkDebugUtilsMessengerEXT debugMessenger;
    VkInstance& _instance;


    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(_instance, debugMessenger, pAllocator);
    }
}


    
};

}//namespace shb