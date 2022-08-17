#include "device.hpp"
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>
#include <cstring>
namespace shb{

void Device::createInstance(){

    if (enableValidationLayers && !_debug->checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }
 
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_0;
    appInfo.pApplicationName = "ShbVkEngineApp";
    appInfo.pEngineName = "ShbVkEngine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.engineVersion =  VK_MAKE_VERSION(1,0,0);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;


    auto extensions = _debug->getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if(enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else{
        createInfo.enabledLayerCount = 0;
    }

    auto res = vkCreateInstance(&createInfo,NULL,&_instance);
    if( res == VK_SUCCESS){
        
        _debug->log("Instance successfully created\n");
    }else{ 
        _debug->log("instance creation failed\n");
        }

}



void Device::initVulkan(){
    
    createInstance();
    //createPhysicalDevice(); //TODO
    
    
}

Device::Device(){
    initVulkan();
}

void Device::cleanup(){
    _debug = nullptr;
    vkDestroyInstance(_instance, NULL);
}

Device::~Device(){
    cleanup();
}

}//namespace shb