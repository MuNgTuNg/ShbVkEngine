#pragma once
#include "device.hpp"
#include <vulkan/vulkan.hpp>


#define IMAGE_COUNT 2


struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};


namespace shb{
class sDevice;
class sSwapchain{
 public:
    sSwapchain(sDevice& d) : _device(d) {}
    void createSwapchain();
    VkSwapchainKHR getSwapchain() { return _swapchain;}

    VkSwapchainKHR _swapchain;
 private:
    sDevice& _device;
    
    sSwapchain* _oldSwapchain = nullptr;
};


}