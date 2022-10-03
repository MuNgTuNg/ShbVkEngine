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

void sSwapchain::createImage(VkImageType imageType, VkFormat format,VkImageTiling tiling,VkSampleCountFlagBits sampling, VkImageUsageFlags usage, int imageIndex,VkDeviceMemory& memory  ){
   //create image
        VkImageCreateInfo imageCreateInfo{};
        imageCreateInfo.imageType = imageType;                                                            //2D or 3D image?
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = NULL;
        imageCreateInfo.format = format;                                                                  //format of image?
        imageCreateInfo.extent.height = static_cast<uint32_t>(_window.height()); //size of y
        imageCreateInfo.extent.width = static_cast<uint32_t>(_window.width());   //size of x
        imageCreateInfo.extent.depth = 1; 
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.tiling = tiling;                                                                  //tiling affects how texels are laid out in image during rendering
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.samples = sampling;                                                               //samples per pixel? (related to MSAA i think)
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.usage = usage;                                                                    //color? depth? other?
        
        vkCreateImage(_device.getDevice(),&imageCreateInfo,nullptr,&_swapchainImages[imageIndex]);        //creates image at imageindex of the images that the swapchain holds
        
      //returns the necessary details to do with allocating memory relating to the object passed into vkGet*MemoryRequirements()
        VkMemoryRequirements req{};
        vkGetImageMemoryRequirements(_device.getDevice(),_swapchainImages[imageIndex],&req);
        
      //info relating to the block of memory you want to allocate
        VkMemoryAllocateInfo memInfo{};
        memInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memInfo.allocationSize = req.size;
      
      //allocate and bind memory
        memory;                                               //handle for memory
        vkAllocateMemory(_device.getDevice(),&memInfo,nullptr,&memory);              //allocate device memory
        vkBindImageMemory(_device.getDevice(),_swapchainImages[imageIndex],memory,0);//bind the memory to the actual image

}



void sSwapchain::createImageViews(){  
  //image views are essentially a frame for an image that helps give the device information ahead of time about the 
  //details of the image it is going to recieve

        _swapchainImageViews.resize(_swapchainImages.size()); //resize member variable for the swapchain image views to hold an image view for every image
        

        int COLOR_IMAGE_INDEX = 0;
       //create color image 
        createImage( VK_IMAGE_TYPE_2D,
                     _format,
                     VK_IMAGE_TILING_OPTIMAL,
                     VK_SAMPLE_COUNT_1_BIT,
                     VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                     COLOR_IMAGE_INDEX,
                     _depthMemory                     );
       _usedImages++;

       int DEPTH_IMAGE_INDEX = 1;
       //create depth image
        createImage( VK_IMAGE_TYPE_2D,
                     VK_FORMAT_D32_SFLOAT,
                     VK_IMAGE_TILING_OPTIMAL,
                     VK_SAMPLE_COUNT_1_BIT,
                     VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                     DEPTH_IMAGE_INDEX,
                     _colorMemory                            );
        _usedImages++;
  
    
        //mip level is essentially downscaling an image e.g miplevel 0 would be 480x480 pixels,  
        //but mip level 1 would be 240x240, mip level 2 would be 120x120#
        //baseMipLevel is the first mip level being used
        //levelCount is the total mip layers being used starting from 0

        //Layer: Images in Vulkan can actually be an array of images, called layers
        //six length texture array. Typically texture arrays simply let you bind 
        //one image to N draws that consume some subset of those layers. Because 
        //many textures for a given game are sized similarly in a given scene and 
        //material, packing those textures and binding once is a performance win.
        //baseArrayLayer & layerCount work as the miplevel and levelcount work

        // Aspect: Images can hold various types of data, in Vulkan either color 
        // (the type of image you'd normally think of), depth (where the image represents 
        // the distance from the camera to objects in the scene), and stencil (a bitmap that 
        // can be used for any purpose). The aspect indicates how it's meant to be used and what type 
        // of data it contains.


      //create resource range
        VkImageSubresourceRange colorResourceRange{};   
        colorResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; 
        colorResourceRange.levelCount = 1;
        colorResourceRange.baseMipLevel = 0; 
        colorResourceRange.baseArrayLayer =0;
        colorResourceRange.layerCount = 1;   
      
      //set components swizzles
        VkComponentMapping components{};             
        components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.b = VK_COMPONENT_SWIZZLE_IDENTITY;

       //struct used to create image view
        VkImageViewCreateInfo colorCreateInfo{};                          
        colorCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;  
        colorCreateInfo.image = _swapchainImages[COLOR_IMAGE_INDEX];      //index of image in vector that is having image view constructed for it
        colorCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; 
        colorCreateInfo.format = _format;                                 //format is the format of the images, can be thought of as a texel/pixel format (e.g VK_FORMAT_B8G8R8A8_SRGB)
        colorCreateInfo.components = components;
        colorCreateInfo.subresourceRange =  colorResourceRange;
        
      //create single image view for indexed image
        if(vkCreateImageView(_device.getDevice(),&colorCreateInfo,nullptr,&_swapchainImageViews[COLOR_IMAGE_INDEX]) != VK_SUCCESS){
            sDebug::Print("Failed to create image view");
        }else{
            sDebug::Print("Successfully created image view");
        } //saf


        VkImageSubresourceRange depthResourceRange{};  
        depthResourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        depthResourceRange.baseArrayLayer = 0;
        depthResourceRange.baseMipLevel = 0;
        depthResourceRange.layerCount = 1;
        depthResourceRange.levelCount = 1;

        //struct used to create image view
        VkImageViewCreateInfo depthCreateInfo{};                          
        depthCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;  
        depthCreateInfo.image = _swapchainImages[DEPTH_IMAGE_INDEX];      //index of image in vector that is having image view constructed for it
        depthCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; 
        depthCreateInfo.format = VK_FORMAT_D32_SFLOAT;                 //format is the format of the images (e.g VK_FORMAT_B8G8R8A8_SRGB)
        depthCreateInfo.components = components;
        depthCreateInfo.subresourceRange =  depthResourceRange;
      
      //create single image view for indexed image
        if(vkCreateImageView(_device.getDevice(),&depthCreateInfo,nullptr,&_swapchainImageViews[DEPTH_IMAGE_INDEX]) != VK_SUCCESS){
            sDebug::Print("Failed to create image view");
        }else{
            sDebug::Print("Successfully created image view");
        } //saf

}


sSwapchain::~sSwapchain(){
  for(int i = 0; i< _usedImages; ++i){ //only using 2 images right now
    sDebug::Print("Destroying image");
    vkDestroyImage(_device.getDevice(),_swapchainImages[i],nullptr);
    
  }

    vkFreeMemory(_device.getDevice(),_colorMemory,nullptr);
    vkFreeMemory(_device.getDevice(),_depthMemory,nullptr);

    for(auto imageView : _swapchainImageViews){
        vkDestroyImageView(_device.getDevice(),imageView,nullptr);
    }
}

}//namespace shb
