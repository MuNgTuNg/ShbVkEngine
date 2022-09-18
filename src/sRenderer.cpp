

#include "sRenderer.hpp"

namespace shb{

void sRenderer::createSwapchain(){
    _swapchain.createSwapchain();
    _swapchain.createImageViews();
    createPipelineLayout();
    createGraphicsPipleine();

}

void sRenderer::createRenderPass(){ //TODO
    auto _imageViews = _swapchain.getImageViews();
    std::vector<VkAttachmentDescription> colorAttachments(_imageViews.size());
    colorAttachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachments[0].format = _swapchain.getFormat();
    colorAttachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachments[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    VkAttachmentReference attachmentReference;
    attachmentReference.attachment = 0;
    attachmentReference.layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    uint32_t subPassCount = 1;
    std::vector<VkSubpassDescription> subPasses(subPassCount);
    subPasses[0].colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());
   // subPasses[0].pColorAttachments = &attachmentReference;


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
}

VkPipelineVertexInputStateCreateInfo sRenderer::setVertexInput() { 

    VkPhysicalDeviceProperties deviceProperties{};
    vkGetPhysicalDeviceProperties(_device.getPhysicalDevice(),&deviceProperties);


    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    VkVertexInputAttributeDescription attributeDescription1;
    attributeDescription1.format = _swapchain.getFormat();

    uint32_t vertAttrOffset = 0;
    uint32_t vertAttrBinding = 0;
    uint32_t vertAttrLocation = 0;

    if(vertAttrOffset <= deviceProperties.limits.maxVertexInputAttributeOffset 
       && vertAttrBinding <= deviceProperties.limits.maxVertexInputBindings   
       && vertAttrLocation <= deviceProperties.limits.maxVertexInputAttributes ){

        attributeDescription1.offset = vertAttrOffset;
        attributeDescription1.binding = vertAttrBinding;
        attributeDescription1.location = vertAttrLocation;

    }else{
        sDebug::Print("Attribute descriptions unsuitable");
    }
    attributeDescriptions.push_back(attributeDescription1);
    
    
  

    std::vector<VkVertexInputBindingDescription> inputBindingDescriptions{};

    VkVertexInputBindingDescription inputBindingDescription1{};

    uint32_t binding = 0;
    uint32_t stride = sizeof(sVertex);
    
    if(binding <= deviceProperties.limits.maxVertexInputBindings 
       && stride <= deviceProperties.limits.maxVertexInputBindingStride){
        inputBindingDescription1.binding = binding;
        inputBindingDescription1.stride = stride;

    }

    inputBindingDescription1.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    inputBindingDescriptions.push_back(inputBindingDescription1);
   

    VkPipelineVertexInputStateCreateInfo inputStateCreateInfo;
    inputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    inputStateCreateInfo.pNext = NULL;
    inputStateCreateInfo.flags = 0;
    inputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    inputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    inputStateCreateInfo.pVertexBindingDescriptions = inputBindingDescriptions.data();
    inputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(inputBindingDescriptions.size());



    return inputStateCreateInfo;
}

void sRenderer::createPipelineLayout(){
    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if(vkCreatePipelineLayout(_device.getDevice(),&createInfo,nullptr,&_pipelineLayout) != VK_SUCCESS){
        sDebug::Print("Failed to create Pipeline Layout");
    }else{
        sDebug::Print("Created pipeline layout");
    }
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

 //VkPipelineCache pipelineCache; //set to VK_NULL_HANDLE, i dont want to use it yet

 createRenderPass();
 VkPipelineVertexInputStateCreateInfo vertexInputState = setVertexInput();

 std::vector<VkGraphicsPipelineCreateInfo> createInfos{}; //todo fill in graphics pipeline create info
 for(int i = 0; i < createInfos.size(); i++){
    createInfos[i].layout = _pipelineLayout;
    // createInfos[i].pColorBlendState =
    // createInfos[i].pDepthStencilState =
    createInfos[i].pDynamicState = NULL;
    // createInfos[i].pInputAssemblyState =
    // createInfos[i].pMultisampleState =
    // createInfos[i].pRasterizationState =
    // createInfos[i].pStages =
    // createInfos[i].pTessellationState =
    createInfos[i].pVertexInputState = &vertexInputState;
    //createInfos[i].renderPass = _renderPass; //not done yet
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
    vkDestroyPipelineLayout(_device.getDevice(),_pipelineLayout,nullptr);

    vkDestroySwapchainKHR(_device.getDevice(), _swapchain.getSwapchain(),nullptr);
}

}//namespace shb