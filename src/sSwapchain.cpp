#include "sSwapchain.hpp"
namespace shb{


void sSwapchain::createSwapchain(){

  //querying swapchain-device details
    uint32_t surfaceFormatsCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceFormatsCount,nullptr);
    std::vector<VkSurfaceFormatKHR> formats(surfaceFormatsCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(_device.getPhysicalDevice(),_device.getSurface(),&surfaceFormatsCount,formats.data());

  //querying details about the surface of the device
    _surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device.getPhysicalDevice(),_device.getSurface(),&_surfaceCapabilities);

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
       sDebug::Print( "Image format: VK_FORMAT_B8G8R8A8_SRGB");
       createInfo.imageColorSpace = format.colorSpace;
        sDebug::Print( "Color space:  VK_COLOR_SPACE_SRGB_NONLINEAR_KHR");
        break;
       }
       
    }
    
  //dealing with assigning image usage, image extent and indices
    int image_count = _surfaceCapabilities.minImageCount + 1;                                        //ideal amount of images is at least the minimum plus one
    if(_surfaceCapabilities.maxImageCount != 0 && image_count < _surfaceCapabilities.maxImageCount){  //if we can have more than one image, and it's less than max
        image_count = _surfaceCapabilities.maxImageCount;                                            //i want maximum images?
    }
    createInfo.minImageCount = _surfaceCapabilities.minImageCount +1;                                //minimum count of swapchain is the minimum the surface can support plus one
    
    createInfo.imageExtent = _surfaceCapabilities.currentExtent;                                     //potential problem if i remember correctly, image extent is set to surfaces image extent
    createInfo.imageArrayLayers = 1;                                                                //image array has one layer (images are represented as arrays)
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;                                       //more than one queue
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;                                    //is being used to write color to the screen 
    createInfo.queueFamilyIndexCount = QFI.queueIndicesArray.size();                                //total indices being used                                   
    createInfo.pQueueFamilyIndices = QFIindeces;                                                    //array of indeces being used
    createInfo.preTransform = _surfaceCapabilities.currentTransform;                                 //pre transformation of screen
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                                  //composite alpha is how the opacity of pixels(texels?) is processed

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
        sDebug::Print("Failed to create swapchain");
    }else{
        sDebug::Print("Created swapchain");
    }
    
  //gather images from swapchain
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(_device.getDevice(),_swapchain,&imageCount,nullptr);
    _swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_device.getDevice(),_swapchain,&imageCount,_swapchainImages.data());  //write them to member vector
    sDebug::Print( "Swapchain Images: "  + std::to_string(_swapchainImages.size()));
    
}


void sSwapchain::createImageViews(){

   
    swapChainImageViews.resize(_swapchainImages.size()); //resize member variable for the swapchain image views to hold an image view for every image
    for(int i =0 ; i<_swapchainImages.size() -1; ++i){      //for each image (image views and images have the same size vector)

      //todo:: this pertains to textures
        // VkImageCreateInfo imageCreateInfo{};
        // imageCreateInfo.imageType = VK_IMAGE_TYPE_3D;
        // imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        // imageCreateInfo.pNext = NULL;
        // imageCreateInfo.format = _format;
        // imageCreateInfo.extent.height = static_cast<uint32_t>(_surfaceCapabilities.currentExtent.height);
        // imageCreateInfo.extent.width = static_cast<uint32_t>(_surfaceCapabilities.currentExtent.width);
        // imageCreateInfo.arrayLayers = 1;
        // imageCreateInfo.mipLevels = 1;
        // imageCreateInfo.extent.depth = 1;
        // imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        // imageCreateInfo.format = _format;
        // imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        // imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        // imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;

        // vkCreateImage(_device.getDevice(),&imageCreateInfo,nullptr,&_swapchainImages[i]);  //todo need to fill this in and create an actual image so that i can create a 3d image view 
      

      //create resource range
        VkImageSubresourceRange resourceRange{};   
        resourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Aspect: Images can hold various types of data, in Vulkan either color 
                                                              // (the type of image you'd normally think of), depth (where the image represents 
                                                              // the distance from the camera to objects in the scene), and stencil (a bitmap that 
                                                              // can be used for any purpose). The aspect indicates how it's meant to be used and what type 
                                                              // of data it contains.

        resourceRange.levelCount = 1;
        resourceRange.baseMipLevel = 0; //mip level is essentially downscaling an image e.g miplevel 0 would be 480x480 pixels,  
                                        //but mip level 1 would be 240x240, mip level 2 would be 120x120#
                                        //baseMipLevel is the first mip level being used
                                        //levelCount is the total mip layers being used starting from 0

        resourceRange.baseArrayLayer =0;//Layer: Images in Vulkan can actually be an array of images, called layers
        resourceRange.layerCount = 1;   //six length texture array. Typically texture arrays simply let you bind 
                                        //one image to N draws that consume some subset of those layers. Because 
                                        //many textures for a given game are sized similarly in a given scene and 
                                        //material, packing those textures and binding once is a performance win.
                                        //baseArrayLayer & layerCount work as the miplevel and levelcount work
        
      
      //set components swizzles
        VkComponentMapping components{};              //investigate further (research)
        components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.b = VK_COMPONENT_SWIZZLE_IDENTITY;

       //struct used to create image view
        VkImageViewCreateInfo createInfo{};                          
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;  
        createInfo.image = _swapchainImages[i];      //index of image in vector that is having image view constructed for it
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // todo will make 3D soon 
        createInfo.format = _format;                 //format is the format of the images (e.g VK_FORMAT_B8G8R8A8_SRGB)
        createInfo.components = components;
        createInfo.subresourceRange =  resourceRange;
      
      //create single image view for indexed image
        if(vkCreateImageView(_device.getDevice(),&createInfo,nullptr,&swapChainImageViews[i]) != VK_SUCCESS){
            sDebug::Print("Failed to create image view");
        }else{
            sDebug::Print("Successfully created image view");
        } //saf
    }



    
}


sSwapchain::~sSwapchain(){
    for(auto imageView : swapChainImageViews){
        vkDestroyImageView(_device.getDevice(),imageView,nullptr);
    }
}

}//namespace shb
