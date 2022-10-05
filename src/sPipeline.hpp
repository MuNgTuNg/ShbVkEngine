#pragma once

#include "sSwapchain.hpp"
#include "sWindow.hpp"
#include "sDevice.hpp"
#include "sCommands.hpp"


#include <glm/glm.hpp>

#include <fstream>
#include <filesystem>
namespace shb{

// • Pipeline class holds the pieces of vulkan used for the pipeline


class sPipeline{
   friend class sRenderer;
 public:
   sPipeline(sWindow& window,sDevice& device, sSwapchain& s );
   ~sPipeline();
  
 private:
   
   
   void createGraphicsPipleine();
   void createRenderPass();
   void createPipelineLayout();
   void createFramebuffers();

   void setVertexInput(std::vector<VkVertexInputAttributeDescription>&  attributeDescriptions,
                       std::vector<VkVertexInputBindingDescription>& inputBindingDescriptions,
                       VkPipelineVertexInputStateCreateInfo& inputStateCreateInfo  );
  
   sWindow& _window;
   sDevice& _device;
   sSwapchain& _swapchain;

   VkPipeline _pipeline;
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