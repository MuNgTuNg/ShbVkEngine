
#include "sDevice.hpp"
#include <cstring>
namespace shb{




static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "\nvalidation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}


std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); //get glfw's necessary extensions then.. 

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount); //add them to a vector 

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);  //debug utils is an instance level extension, always use macro instead of actual string as allows for easier updates 
        //extensions.push_back("VK_KHR_surface");                 // surface is a device level extension i think?
    }

    return extensions;                                            //return vector to be used externally
}


bool sDevice::checkValidationLayerSupport() {
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


  void sDevice::createLogicalDevice() { 

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
    devCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    devCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    

    if(vkCreateDevice(_gpu,&devCreateInfo,nullptr,&_device)!= VK_SUCCESS){
      std::cout<<"Device creation failed\n";
    }else{
      std::cout<<"Device creation success\n";
    }

    vkGetDeviceQueue(_device,indeces.graphicsFamily, 0, &_graphicsQueue);
  }


  
 //retrieves and returns a struct holding the indices of the queue families on the device 
  QueueFamilyIndices sDevice::findQueueFamilies(VkPhysicalDevice device){
    QueueFamilyIndices QFI;  //tuple like data structure

  //query queue family properties
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueCount,nullptr);
    std::vector<VkQueueFamilyProperties> properties(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueCount,properties.data());

    
  //run through properties and check for desired properties stored in VkQueueFamilyProperties object 
    int i = 0;                                                                    //set index variable to 0
    for(const auto& property : properties){                                       //loop through each property

      VkBool32 presentSupported; //boolean returned to query for surface support
      vkGetPhysicalDeviceSurfaceSupportKHR(device,i,_surface,&presentSupported); //does the queue at index i support presentation?

      if(property.queueCount > 0 && property.queueFlags & VK_QUEUE_GRAPHICS_BIT){ //if supports more than 0 queues and has a queue flag of VK_QUEUE_GRAPHICS_BIT (supports graphics)
        QFI.graphicsFamily = i;                                                      //set graphics family index to the current iteration's index value
        QFI.queueIndicesArray.push_back(i);                                          //add to vector of queueIndeces
        QFI.gfxFamilyHasValue = true;                                                //change boolean to say that graphicsFamily index has been assigned to
        i++; //for concurrency, queue family indeces must be different
        
      }
      if(property.queueCount > 0 && presentSupported){                            //if supports more than 0 queues and the queue that is on the current index supports a physical device surface 
        QFI.presentFamily = i;                                                    //same process as last time, just with present family
        QFI.queueIndicesArray.push_back(i);
        QFI.presentFamilyHasValue = true;
        i++;
      }
      
      if(QFI.isComplete()){                                                       //if we have fullfilled our requirements, break from for loop
        break;
      }
      i++;                                                                        //if not, loop over again with the queue index advanced by 1
      
    }
    QFI.totalQueues = i;                                                          //totalQueues could be replaced by queueArray.size(), returns the number of times loop was iterated through not how many there were
    return QFI;                                                                   //return object

  }

//choose what features  we want from the device and assign a device based on suitability
  bool sDevice::isDeviceSuitable(VkPhysicalDevice& device) {

    QueueFamilyIndices QFI = findQueueFamilies(device);     //locate queue families on device, found through retrieving an index that is used as a handle for that queue
    
    return QFI.isComplete();                                //query completion of retrieval of desired queues and signals that a suitable device was found if complete


  }

sDevice::~sDevice(){

  vkDestroyDevice(_device,nullptr);
  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(_instance, debugMessenger, nullptr);     //debug must be destroyed before the instance as it relies on it
  }
  vkDestroySurfaceKHR(_instance,_surface, nullptr);      
  vkDestroyInstance(_instance, nullptr);

}






}//namespace shb