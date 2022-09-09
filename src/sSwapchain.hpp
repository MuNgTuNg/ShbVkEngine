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
class sDevice;
class sSwapchain{
 public:
    sSwapchain(sDevice& d) : _device(d) {}
    ~sSwapchain();
    void createSwapchain();
    void createImageViews();
    VkSwapchainKHR getSwapchain() { return _swapchain;}
    

    VkSwapchainKHR _swapchain;
 private:
    sDevice& _device;
    
    sSwapchain* _oldSwapchain = nullptr;
    std::vector<VkImage> _swapchainImages;

    VkPresentModeKHR _presentMode = VK_PRESENT_MODE_FIFO_KHR; //todo:: query available present modes
    VkFormat _format;
    std::vector<VkImageView> swapChainImageViews;

};


}