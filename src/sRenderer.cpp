

#include "sRenderer.hpp"

namespace shb{

void sRenderer::createSwapchain(){
    _swapchain.createSwapchain();
    _swapchain.createImageViews();
    createGraphicsPipleine();

}


void sRenderer::createGraphicsPipleine(){
// • The state required for a graphics pipeline is divided into:
//    » Vertex input state
//    » Pre-rasterization shader state
//    » Fragment shader state
//    » Fragment output state

// • Vertex input state is defined by:
//    » VkPipelineInputAssemblyStateCreateInfo
//    » VkPipelineVertexInputStateCreateInfo

// • Pre-rasterization shader state is defined by:
//    » VkPipelineShaderStageCreateInfo entries for:
//       - Vertex shaders
//       - Tessellation control shaders
//       - Tessellation evaluation shaders
//       - Geometry shaders
//       - Task shaders
//       - Mesh shaders
//


// • Fragment shader state is defined by:
//   »

// • Fragment output state is defined by:
//   »
auto _imageViews = _swapchain.getImageViews();
std::vector<VkAttachmentDescription> colorAttachments(_imageViews.size());
colorAttachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
colorAttachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
colorAttachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
colorAttachments[0].format = _swapchain.getFormat();
colorAttachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
colorAttachments[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

uint32_t subPassCount = 1;
std::vector<VkSubpassDescription> subPasses(subPassCount);
subPasses[0].colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());

 //VkPipelineCache pipelineCache; //set to VK_NULL_HANDLE, i dont want to use it yet
 VkRenderPassCreateInfo renderPassCreateInfo{};
 renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
 renderPassCreateInfo.pNext = nullptr;
 renderPassCreateInfo.pAttachments = colorAttachments.data();

 renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subPasses.size());
 renderPassCreateInfo.pSubpasses = subPasses.data();

//  if(vkCreateRenderPass(_device.getDevice(), &renderPassCreateInfo,nullptr, &_renderPass) != VK_SUCCESS){
//     std::cout<<"Failed to create render pass\n";
//  }else{
//     std::cout<<"Successfully created render pass\n";
//  }


 std::vector<VkGraphicsPipelineCreateInfo> createInfos{}; //todo fill in graphics pipeline create info
 for(int i = 0; i < createInfos.size(); i++){
    // createInfos[i].basePipelineHandle =
    // createInfos[i].basePipelineIndex =
    // createInfos[i].layout =
    // createInfos[i].pColorBlendState =
    // createInfos[i].pDepthStencilState =
    createInfos[i].pDynamicState = NULL;
    // createInfos[i].pInputAssemblyState =
    // createInfos[i].pMultisampleState =
    // createInfos[i].pRasterizationState =
    // createInfos[i].pStages =
    // createInfos[i].pTessellationState =
    // createInfos[i].pVertexInputState =
    // createInfos[i].renderPass =
    // createInfos[i].stageCount =
    // createInfos[i].sType =
     //createInfos[i].subpass = subpasses[0];

 }


//  if(vkCreateGraphicsPipelines(_device.getDevice(),VK_NULL_HANDLE,static_cast<uint32_t>(createInfos.size()),createInfos.data(),nullptr,&_pipeline) != VK_SUCCESS){
//     std::cout<<"Failed to initialise graphics pipelines\n";
//  }else{
//     std::cout<<"Successfully initialised graphics pipelines\n";
//  }
}


sRenderer::sRenderer(sWindow& window,sDevice& device)  : _window(window), _device(device), _swapchain(device) {
    createSwapchain();

}

sRenderer::~sRenderer(){

    vkDestroySwapchainKHR(_device.getDevice(), _swapchain.getSwapchain(),nullptr);
}

}//namespace shb