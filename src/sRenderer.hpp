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

 private:
   sWindow& _window;
   sDevice& _device;
   sSwapchain _swapchain{_device};
   std::vector<VkImageView> imageViews;
   

};

}//namespace shb