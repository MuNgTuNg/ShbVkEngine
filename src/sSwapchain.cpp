#include "sSwapchain.hpp"
namespace shb{


void sSwapchain::createSwapchain(){

  //querying swapchain-device details
    uint32_t surfaceFormatsCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceFormatsCount,nullptr);
    std::vector<VkSurfaceFormatKHR> formats(surfaceFormatsCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceFormatsCount,formats.data());

  //querying details about the surface of the device
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceCapabilities);

  //retrieve the queue families indices
    QueueFamilyIndices QFI = _device.findQueueFamilies(_device.getPhysicalDevice());
    uint32_t QFIindeces[] = {QFI.graphicsFamily, QFI.presentFamily};

  //query present modes of device surface
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_device.getPhysicalDevice(),_device.getSurface(),&presentModeCount,nullptr);
    std::vector<VkPresentModeKHR>  presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(_device.getPhysicalDevice(),_device.getSurface(),&presentModeCount,presentModes.data());
  
  //now we have found out enough details about the swapchain we can choose what we want it to do, and make sure the device we're using
  //supports the functionality we intend to use

  //fill in details about the kind of swapchain we want to create
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType =  VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _device.getSurface();
    
  //iterate through all formats that the surface supports
    for(const auto& format : formats){
        
       if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR  ){ //do you support x and y?
       createInfo.imageFormat = format.format;
       _format = format.format;                                                                   //if so i want to use them
       std::cout<< "Image format: " << "VK_FORMAT_B8G8R8A8_SRGB\n";
       createInfo.imageColorSpace = format.colorSpace;
        std::cout<< "Color space: " << "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR\n";
        break;
       }
       
    }
    
  //dealing with assigning image usage, image extent and indices
    int image_count = surfaceCapabilities.minImageCount + 1;                                        //ideal amount of images is at least the minimum plus one
    if(surfaceCapabilities.maxImageCount != 0 && image_count < surfaceCapabilities.maxImageCount){  //if we can have more than one image, and it's less than max
        image_count = surfaceCapabilities.maxImageCount;                                            //i want maximum images?
    }
    createInfo.minImageCount = surfaceCapabilities.minImageCount +1;                                //minimum count of swapchain is the minimum the surface can support plus one
    
    createInfo.imageExtent = surfaceCapabilities.currentExtent;                                     //potential problem if i remember correctly, image extent is set to surfaces image extent
    createInfo.imageArrayLayers = 1;                                                                //image array has one layer (research)
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;                                       //more than one queue
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;                                    //is being used to write color to the screen (research)
    createInfo.queueFamilyIndexCount = QFI.queueIndicesArray.size();                                //total indices being used                                   
    createInfo.pQueueFamilyIndices = QFIindeces;                                                    //array of indeces being used
    createInfo.preTransform = surfaceCapabilities.currentTransform;                                 //pre transformation of screen
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                                  //(research)

  //choose present mode (just the first one i see; sort of crude)
    // for(const auto& modes : presentModes){      
    //     if(modes == VK_PRESENT_MODE_MAILBOX_KHR){
    //         createInfo.presentMode = modes;
    //     }else if(modes == VK_PRESENT_MODE_FIFO_KHR){
    //         createInfo.presentMode = modes;
    //     }
        
    // }
  //choose using the header file just so i can change it from one place  (more crude but makes it easier to feel around a bit)
    createInfo.presentMode = _presentMode;

    createInfo.clipped = VK_TRUE;      //dont draw things under windows or offscreen
    if(_oldSwapchain == nullptr){      //if old swapchain is null it stays null otherwise ?? 
    createInfo.oldSwapchain = VK_NULL_HANDLE;  //(research when recreating swapchain)
    }else{
    createInfo.oldSwapchain = _oldSwapchain->_swapchain;
    }
    
  //create swapchain
    if(vkCreateSwapchainKHR(_device.getDevice(),&createInfo,nullptr,&_swapchain) != VK_SUCCESS){
        std::cout<<"Failed to create swapchain\n";
    }else{
        std::cout<<"Created swapchain\n";
    }
    
  //gather images from swapchain
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(_device.getDevice(),_swapchain,&imageCount,nullptr);
    _swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_device.getDevice(),_swapchain,&imageCount,_swapchainImages.data());  //write them to member vector
    std::cout<< "Swapchain Images: " << _swapchainImages.size() << std::endl;
    
}


void sSwapchain::createImageViews(){

   
    swapChainImageViews.resize(_swapchainImages.size());
    for(int i =0 ; i<_swapchainImages.size(); ++i){
      //todo:: fill out struct
        // VkImageCreateInfo imageCreateInfo{};
        // imageCreateInfo.imageType = VK_IMAGE_TYPE_3D;

        // vkCreateImage(_device.getDevice(),&imageCreateInfo,nullptr,&_swapchainImages[i]);

        VkImageSubresourceRange resourceRange{};
        resourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        resourceRange.baseMipLevel = 0;
        resourceRange.baseArrayLayer =0;
        resourceRange.layerCount = 1;
        resourceRange.levelCount = 1;

        VkComponentMapping components{};
        components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.b = VK_COMPONENT_SWIZZLE_IDENTITY;

        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapchainImages[i];
        
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; //will make 3D soon 
        createInfo.format = _format;
        createInfo.components = components;
        createInfo.subresourceRange =  resourceRange;

        if(vkCreateImageView(_device.getDevice(),&createInfo,nullptr,&swapChainImageViews[i]) != VK_SUCCESS){
            std::cout<<"Failed to create image view\n";
        }else{
            std::cout<<"Successfully created image view\n";
        } //saf
    }



    
}


sSwapchain::~sSwapchain(){
    for(auto imageView : swapChainImageViews){
        vkDestroyImageView(_device.getDevice(),imageView,nullptr);
    }
}

}//namespace shb
