#pragma once

#include "sSwapchain.h"
#include "window.hpp"
#include "device.hpp"

namespace shb{

class sRenderer{
 public:
   sRenderer(sWindow& window,sDevice& device );
   ~sRenderer();
   void createSwapchain();

 private:
   sWindow& _window;
   sDevice& _device;
   sSwapchain _swapchain{_device};
   std::vector<VkImageView> imageViews;
   

};

}//namespace shb