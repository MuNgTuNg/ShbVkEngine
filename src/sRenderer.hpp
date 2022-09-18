#pragma once

#include "sSwapchain.hpp"
#include "sWindow.hpp"
#include "sDevice.hpp"
#include <glm/glm.hpp>

namespace shb{

// • Renderer class holds the pieces of vulkan used for drawing the actual images and sending them to the screen


class sRenderer{
 public:
   sRenderer(sWindow& window,sDevice& device );
   ~sRenderer();
   void createSwapchain();
   void createGraphicsPipleine();
   void createRenderPass();
   void createPipelineLayout();

   VkPipelineVertexInputStateCreateInfo setVertexInput();
  

 private:
   sWindow& _window;
   sDevice& _device;
   sSwapchain _swapchain{_device};
   VkPipeline _pipeline;
   VkRenderPass _renderPass;
   VkPipelineLayout _pipelineLayout;
   

};

class sVertex{
  glm::mat4 pos;
  glm::mat4 color;
};

}//namespace shb