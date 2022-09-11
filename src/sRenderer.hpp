#pragma once

#include "sSwapchain.hpp"
#include "sWindow.hpp"
#include "sDevice.hpp"

namespace shb{

class sRenderer{
 public:
   sRenderer(sWindow& window,sDevice& device );
   ~sRenderer();
   void createSwapchain();
   void createGraphicsPipleine();

 private:
   sWindow& _window;
   sDevice& _device;
   sSwapchain _swapchain{_device};
   std::vector<VkImageView> imageViews;
   VkPipeline _pipeline;
   

};

}//namespace shb