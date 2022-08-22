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


    _extensions = _debug->getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(_extensions.size());
    createInfo.ppEnabledExtensionNames = _extensions.data();

    if(enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(_debug->validationLayers.size());
        createInfo.ppEnabledLayerNames = _debug->validationLayers.data();
    } else{
        createInfo.enabledLayerCount = 0;
    }

    auto res = vkCreateInstance(&createInfo,NULL,&_instance);
    if( res == VK_SUCCESS){
        
        _debug->log("Instance successfully created");
    }else{ 
        _debug->log("instance creation failed");
        }

}



void Device::initVulkan(){
    
    createInstance();
    //createPhysicalDevice(); //TODO
    createLogicalDevice();
    
    
}

void Device::createLogicalDevice(){

}
void Device::createPhysicalDevice(){
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, &_physicalDevice);

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(_physicalDevice, &properties);
	_debug->log("retrieved physical device properties");
       
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, &_queueFamProps);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = NULL;
    queueCreateInfo.flags = VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT;
    //queueCreateInfo.pQueuePriorities = 
    //queueCreateInfo.queueFamilyIndex = 

	VkDeviceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.ppEnabledExtensionNames = _extensions.data();
    //createInfo.pEnabledFeatures = 
    //createInfo.enabledLayerCount = 
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    //createInfo.queueCreateInfoCount =
    //createInfo.ppEnabledLayerNames = 
    //createInfo.pQueueCreateInfos =
    
    //TODO: Finish creating device.
    if(vkCreateDevice(_physicalDevice, &createInfo, NULL, &_device) != VK_SUCCESS){
        _debug->log("Failed to create logical device!");
    }


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
