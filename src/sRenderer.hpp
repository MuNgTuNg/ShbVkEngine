#pragma once

#include "sSwapchain.hpp"
#include "sWindow.hpp"
#include "sDevice.hpp"
#include "sCommands.hpp"


#include <glm/glm.hpp>

#include <fstream>
#include <filesystem>
namespace shb{

// • Renderer class holds the pieces of vulkan used for drawing the actual images and sending them to the screen


class sRenderer{
 public:
   sRenderer(sWindow& window,sDevice& device );
   ~sRenderer();
   void render();

  
  
 private:
   
   void createSwapchain();
   void createGraphicsPipleine();
   void createRenderPass();
   void createPipelineLayout();
   void createFramebuffers();

   void setVertexInput(std::vector<VkVertexInputAttributeDescription>&  attributeDescriptions,
                       std::vector<VkVertexInputBindingDescription>& inputBindingDescriptions,
                       VkPipelineVertexInputStateCreateInfo& inputStateCreateInfo  );
  
   sWindow& _window;
   sDevice& _device;
   sSwapchain _swapchain{_device,_window};
   VkPipeline _pipeline;
   sCommands _commands{_device,_swapchain,_pipeline};

   VkPipelineLayout _pipelineLayout;
   VkRenderPass _renderPass;
   std::vector<VkSubpassDescription> _subPasses{};
   

   VkShaderModule _vertexShader;
   VkShaderModule _fragmentShader;
   const std::string& _vertexShaderLocation{"../shaders/vertShader.vert.spv"}; 
   const std::string& _fragmentShaderLocation{"../shaders/fragShader.frag.spv"};
   VkShaderModule createShaderModule(const std::string& filePath);

  

   std::vector<VkAttachmentReference> _colorAttachments{};
   std::vector<VkAttachmentReference> _depthAttachments{};
   std::vector<VkAttachmentDescription> _attachmentDescriptions{};



 

};



}//namespace shb