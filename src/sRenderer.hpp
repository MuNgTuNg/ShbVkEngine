#pragma once

#include "sSwapchain.hpp"
#include "sWindow.hpp"
#include "sDevice.hpp"

namespace shb{

// • Renderer class holds the pieces of vulkan used for drawing the actual images and sending them to the screen


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
   VkPipeline _pipeline;
   VkRenderPass _renderPass;
   

};

}//namespace shb