#pragma once

#include "sSwapchain.hpp"
#include "sWindow.hpp"
#include "sDevice.hpp"

#include <glm/glm.hpp>

#include <fstream>
#include <filesystem>
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

   void setVertexInput(std::vector<VkVertexInputAttributeDescription>&  attributeDescriptions,
                       std::vector<VkVertexInputBindingDescription>& inputBindingDescriptions,
                       VkPipelineVertexInputStateCreateInfo& inputStateCreateInfo  );
  
   VkShaderModule createShaderModule(const std::string& filePath);

 private:
   sWindow& _window;
   sDevice& _device;
   sSwapchain _swapchain{_device};
   VkPipeline _pipeline;
   VkRenderPass _renderPass;
   VkPipelineLayout _pipelineLayout;
   std::vector<VkSubpassDescription> _subPasses{};

   VkShaderModule _vertexShader;
   VkShaderModule _fragmentShader;
   const std::string& _vertexShaderLocation{"../shaders/vertShader.vert.spv"}; 
   const std::string& _fragmentShaderLocation{"../shaders/fragShader.frag.spv"};

 

};

class sVertex{
  float posX, posY, posZ;
  uint32_t r,g,b;
};

}//namespace shb