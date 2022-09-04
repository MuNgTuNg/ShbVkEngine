#include "sSwapchain.h"
namespace shb{


void sSwapchain::createSwapchain(){

    SwapChainSupportDetails swapChainSupport; 
    swapChainSupport.formats


    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceCapabilities);
    QueueFamilyIndices QFI = _device.findQueueFamilies(_device.getPhysicalDevice());
    uint32_t QFIindeces[] = {QFI.graphicsFamily, QFI.presentFamily};
     

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType =  VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _device.getSurface();
    createInfo.imageFormat = VK_FORMAT_R8G8B8_SRGB;

    int image_count = surfaceCapabilities.minImageCount + 1;
    if(surfaceCapabilities.maxImageCount != 0 && image_count < surfaceCapabilities.maxImageCount){
        image_count = surfaceCapabilities.maxImageCount;
    }
    createInfo.minImageCount = surfaceCapabilities.minImageCount +1;
    
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = surfaceCapabilities.currentExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = QFIindeces;
    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

    createInfo.clipped = VK_TRUE;
    if(_oldSwapchain == nullptr){
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    }else{
    createInfo.oldSwapchain = _oldSwapchain->_swapchain;
    }
    
    if(vkCreateSwapchainKHR(_device.getDevice(),&createInfo,nullptr,&_swapchain) != VK_SUCCESS){
        std::cout<<"Failed to create swapchain\n";
    }else{
        std::cout<<"Created swapchain\n";
    }
    
}



}//namespace shb
