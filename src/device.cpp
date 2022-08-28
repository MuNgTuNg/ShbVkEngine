
#include "device.hpp"
#include <cstring>
namespace shb{


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}


std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        //extensions.push_back("VK_KHR_surface");
    }

    return extensions;
}


bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;
    
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
    
        if (!layerFound) {
            return false;
        }
}

return true;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void sDevice::setupDebugMessenger() {
   if (!enableValidationLayers) return;
   
   
   VkDebugUtilsMessengerCreateInfoEXT createInfo;
   populateDebugMessengerCreateInfo(createInfo);


   if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
}

}




  void sDevice::initVulkan() {
    createInstance();
    setupDebugMessenger();
    _window.createSurface(_instance, &_surface);
    pickPhysicalDevice();
    
    createLogicalDevice();

  }

  
  void sDevice::createInstance() {

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }



    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


    
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Application 1";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "ShbVkEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();




    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }else{
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

  
    if(vkCreateInstance(&createInfo,nullptr,&_instance)== VK_SUCCESS){
      std::cout<<"Successfully created Instance\n";
    }else{
      std::cout<<"Failed to create instance\n";
    }
  }



  void sDevice::pickPhysicalDevice() {

    uint32_t pdCount=0;
    vkEnumeratePhysicalDevices(_instance,&pdCount, nullptr);

    std::vector<VkPhysicalDevice> devices(pdCount);
    vkEnumeratePhysicalDevices(_instance,&pdCount,devices.data());

    for(auto device : devices){
      if(isDeviceSuitable(device)){
        _gpu = device;
        break;
      }
    }
    if(_gpu == VK_NULL_HANDLE){
      std::cout<<"Unable to find a GPU\n";
    }else{
      std::cout<<"Found suitable GPU!\n";
    }


  }


  void sDevice::createLogicalDevice() { //TODO finish device setup

    QueueFamilyIndices indeces = findQueueFamilies(_gpu);

    VkPhysicalDeviceFeatures features{};

    VkDeviceQueueCreateInfo queCreateInfo {};
    queCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queCreateInfo.queueFamilyIndex = indeces.graphicsFamily;
    queCreateInfo.queueCount = 1;
    float priorities = 1.0f;
    queCreateInfo.pQueuePriorities = &priorities;
    queCreateInfo.pNext = nullptr;
    
    VkDeviceCreateInfo devCreateInfo {};
    devCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devCreateInfo.pQueueCreateInfos = &queCreateInfo;
    devCreateInfo.queueCreateInfoCount = 1;
    devCreateInfo.pEnabledFeatures = &features;
    devCreateInfo.enabledExtensionCount = 0;
    

    if(vkCreateDevice(_gpu,&devCreateInfo,nullptr,&_device)!= VK_SUCCESS){
      std::cout<<"Device creation failed\n";
    }else{
      std::cout<<"Device creation success\n";
    }

    vkGetDeviceQueue(_device,indeces.graphicsFamily, 0, &_graphicsQueue);
  }


  

  QueueFamilyIndices sDevice::findQueueFamilies(VkPhysicalDevice device){
    QueueFamilyIndices QFI;

    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueCount,nullptr);
    std::vector<VkQueueFamilyProperties> properties(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueCount,properties.data());
    
    int i = 0;
    for(auto property : properties){
      if(property.queueFlags & VK_QUEUE_GRAPHICS_BIT){
        QFI.graphicsFamily = i;
        QFI.gfxFamilyHasValue = true;
      }
      i++;
    }
    QFI.presentFamilyHasValue = true; //not true, temperary

    return QFI;

  }

  bool sDevice::isDeviceSuitable(VkPhysicalDevice& device) {

    QueueFamilyIndices QFI = findQueueFamilies(device);
    
    return QFI.isComplete();


  }

sDevice::~sDevice(){

  vkDestroyDevice(_device,nullptr);
  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(_instance, debugMessenger, nullptr);
  }
  vkDestroySurfaceKHR(_instance,_surface, nullptr);
  vkDestroyInstance(_instance, nullptr);

}






}//namespace shb