#include "device.hpp"
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>
#include <cstring>
#include <string>
namespace shb{



static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData) {
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl <<std::endl;
  return VK_FALSE;
}
    VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance,
      "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}


    void Device::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;  // Optional
}

void Device::setupDebugMessenger() {
  if (!enableValidationLayers) return;
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);
  if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance,
      "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

bool Device::checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
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

std::vector<const char *> Device::getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if (enableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    extensions.push_back("VK_KHR_surface");
  }

  return extensions;
}


void Device::createInstance() {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Shay's App";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto extensions = getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  if (enableValidationLayers) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
    _debug->log("Failed to create instance");
  }
  else {
    _debug->log("Created instance successfully");
  }

  //hasGflwRequiredInstanceExtensions();
}



void Device::initVulkan(){
    createInstance();
    setupDebugMessenger();
    pickPhysicalDevice(); 
    createLogicalDevice();
    createSurface();
    
    
}

void Device::createSurface(){

  //TODO: create surface (XCB (ballache) or figure out workaround)

  if(glfwCreateWindowSurface(_instance,_window->getwindow(),NULL,&_surface)!= VK_SUCCESS){
    _debug->log("Surface Not Created");
  }else{
    _debug->log("Surface Successfully created");
  }
}


void Device::createLogicalDevice(){
    QueueFamilyIndices indices = findQueueFamilies(_physicalDevice); //finds the queue families of the assigned physical device

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily; //the index of the graphics queue found by findQueueFamilies
    queueCreateInfo.queueCount = 1;  //we have one queue thus far

    //priority is the priority of execution of the queue being created, 1.0f is max priority
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{}; //device features (Multisample rate, Depthclamp, Geometry Shader, etc.)
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    //deprecated
    createInfo.enabledExtensionCount = 0;
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if(vkCreateDevice(_physicalDevice,&createInfo,nullptr,&_device) != VK_SUCCESS){
        _debug->log("Failed to create logical device");
    }
    else{
      _debug->log("Successfully created logical device!");
    }

    vkGetDeviceQueue(_device,indices.graphicsFamily, 0, &graphicsQueue);




}

//logic to find graphics queue family
QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device) { 
  QueueFamilyIndices indices;

  uint32_t queueCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr); //get amount of queue families supported

  std::vector<VkQueueFamilyProperties> properties(queueCount); //create vector to hold them
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, properties.data()); //write them into vector

  uint32_t i = 0;
  for(const auto & property : properties){
    if(property.queueFlags & VK_QUEUE_GRAPHICS_BIT){  //does our device support graphics queues?
      indices.graphicsFamily = i;       //if so set its index to the iteration number
      indices.graphicsFamilyHasValue = true; //also set hasvalue to true
    }
    ++i;  //increment index
  }
  
  return indices; //return bundled information

}


bool Device::isDeviceSuitable(VkPhysicalDevice device){
    // VkPhysicalDeviceProperties properties;         //list properties of device
    // vkGetPhysicalDeviceProperties(device, &properties);
    // VkPhysicalDeviceFeatures features;            //list features of device
    // vkGetPhysicalDeviceFeatures(device, &features);
    // _debug->log(properties.deviceName);
   

    // return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
    // && features.geometryShader;
        
    QueueFamilyIndices indices = findQueueFamilies(device);
    return indices.isComplete();
}

void Device::pickPhysicalDevice(){
    uint32_t deviceCount;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr); //coming up with some weird error
   
    if(deviceCount == 0){
       _debug->log("No GPU found");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
   
    vkEnumeratePhysicalDevices(_instance,&deviceCount,devices.data());
    
    for(const auto& device : devices){
        if(isDeviceSuitable(device)){
            _physicalDevice = device;
            _debug->log("Found GPU" );
            break;
        }
    }
    if(_physicalDevice == VK_NULL_HANDLE){
        _debug->log("No suitable device found!");
    }

    
}



void Device::cleanup(){
    
    _debug = nullptr;
    vkDestroyDevice(_device, nullptr);
    
    if(enableValidationLayers){
      DestroyDebugUtilsMessengerEXT(_instance,debugMessenger,nullptr);
    }


    vkDestroyInstance(_instance, NULL);
}

Device::~Device(){
    cleanup();
}

}//namespace shb
