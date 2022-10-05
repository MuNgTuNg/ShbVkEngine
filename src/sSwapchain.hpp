#pragma once
#include "sDevice.hpp"
#include <vulkan/vulkan.hpp>


#define IMAGE_COUNT 2

//struct designed to keep all details about swapchain, i just did it in class  because it's easier to read
// struct SwapChainSupportDetails {
//   VkSurfaceCapabilitiesKHR capabilities;
//   std::vector<VkSurfaceFormatKHR> formats;
//   std::vector<VkPresentModeKHR> presentModes;
// };


namespace shb{

class sWindow;
class sDevice;
class sSwapchain{
   friend class sRenderer;
 public:
    sSwapchain(sDevice& d, sWindow& w) : _device(d), _window(w) {}
    ~sSwapchain();
    void createSwapchain();
    void createImageViews();
    VkSwapchainKHR getSwapchain() { return _swapchain;}
    VkFormat getFormat() { return _format; }
    std::vector<VkImageView>& getImageViews() { return _swapchainImageViews;}
    VkFramebuffer getFrameBuffer(int i) { return _frameBuffers[i]; }

    void createImage( VkImageType imageType, 
                                  VkFormat format,
                                  VkImageTiling tiling,
                                  VkSampleCountFlagBits sampling, 
                                  VkImageUsageFlags usage, 
                                  int imageIndex,
                                  VkDeviceMemory& memory  );

        std::vector<VkFramebuffer> _frameBuffers{};
        std::vector<VkImageView> _swapchainImageViews;


   VkSurfaceCapabilitiesKHR _surfaceCapabilities;
    VkSwapchainKHR _swapchain;
 private:
    sDevice& _device;
    sWindow& _window;

    sSwapchain* _oldSwapchain = nullptr;
    std::vector<VkImage> _swapchainImages;
   
    
    
    VkPresentModeKHR _presentMode = VK_PRESENT_MODE_FIFO_KHR; //todo:: query available present modes
    VkFormat _format;
    int _usedImages = 0;
    VkDeviceMemory _colorMemory;
    VkDeviceMemory _depthMemory;    

};


}