

#include "sRenderer.hpp"

namespace shb{

void sRenderer::createSwapchain(){
    _swapchain.createSwapchain();
    _swapchain.createImageViews();
}



void sRenderer::createRenderPass(){ 
   //add colorAttachments vector
    std::vector<VkAttachmentDescription> colorAttachmentsDescriptions{};
    
   //keep an index of each color attachment 
   int colorAttachmentIndex = 0;
    VkAttachmentDescription colorAttachmentDescription{};
    colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;     //image starts undefined
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //i want this image to be presentable when the render pass is finished
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;               //how many samples (per pixel?) i want to take
    colorAttachmentDescription.format = _swapchain.getFormat();               //format of the image being described
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;          //when i load this attachment i want it to clear its contents
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;    //i dont care about storing it, it doesnt matter right now

   //add color attachment to the vector of color attachments 
    colorAttachmentsDescriptions.push_back(colorAttachmentDescription);

   //handles for color attachments 
    std::vector<VkAttachmentReference> colorAttachments{};
  
  //first color attachment
    VkAttachmentReference colorAttachment1;
    colorAttachment1.attachment = colorAttachmentIndex;                   //index of the particular attachment
    colorAttachment1.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;   //how i want the image laid out
    colorAttachments.push_back(colorAttachment1);
  

  //assuming this will be needed later
    std::vector<VkAttachmentDescription> depthAttachmentDescription{};

  //create subpass 1 (0)
    VkSubpassDescription subPass1{};
    subPass1.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;    //this is going to bound to a graphics pipeline
    subPass1.colorAttachmentCount = colorAttachments.size();         //all the color attachments
    subPass1.pColorAttachments = colorAttachments.data();            //pointer into the color attachments vector
    _subPasses.push_back(subPass1);

  //render pass information
     VkRenderPassCreateInfo renderPassCreateInfo{};
     renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
     renderPassCreateInfo.pNext = nullptr;  
     renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(colorAttachmentsDescriptions.size());
     renderPassCreateInfo.pAttachments = colorAttachmentsDescriptions.data();    
     renderPassCreateInfo.subpassCount = static_cast<uint32_t>(_subPasses.size());
     renderPassCreateInfo.pSubpasses = _subPasses.data();
     

     if(vkCreateRenderPass(_device.getDevice(), &renderPassCreateInfo,nullptr, &_renderPass) != VK_SUCCESS){
        sDebug::Print("Failed to create render pass");
      }else{
        sDebug::Print("Successfully created render pass");
     }
}



void sRenderer::setVertexInput(std::vector<VkVertexInputAttributeDescription>&  attributeDescriptions,
                               std::vector<VkVertexInputBindingDescription>& inputBindingDescriptions,
                               VkPipelineVertexInputStateCreateInfo& inputStateCreateInfo            ) {  //TODO:: annotate

    VkPhysicalDeviceProperties deviceProperties{};
    vkGetPhysicalDeviceProperties(_device.getPhysicalDevice(),&deviceProperties);

    VkVertexInputAttributeDescription attributeDescription1{};
    attributeDescription1.format = VK_FORMAT_R32G32_SFLOAT; //confusingly this uses the same enum as the colorspace
    attributeDescription1.binding = 0;
    attributeDescription1.location = 0;
    //attributeDescription1.offset = offsetof(sVertex, pos);

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
   

    inputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    inputStateCreateInfo.pNext = NULL;
    inputStateCreateInfo.flags = 0;
    inputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    inputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    inputStateCreateInfo.pVertexBindingDescriptions = inputBindingDescriptions.data();
    inputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(inputBindingDescriptions.size());



}



void sRenderer::createPipelineLayout(){ //not entirely sure what this is used for yet
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


//do the necessary things to create a renderpass
 createRenderPass();


//create vectors necessary for the inputStateCreateInfos to point to, if you don't pass them in they get deleted and inputstateCreateInfo points to nothing
 std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
 std::vector<VkVertexInputBindingDescription> inputBindingDescriptions{};
 VkPipelineVertexInputStateCreateInfo inputStateCreateInfo {};

//make all necessary settings to structs in order to decide how vertices are inputted to the (shader programme?)
 setVertexInput(attributeDescriptions,inputBindingDescriptions,inputStateCreateInfo); //TODO:: abstract this into a static function in a vertex class, it belongs to vertex


//fill in information about pipelines wanting to be created
 std::vector<VkGraphicsPipelineCreateInfo> pipelineCreateInfos{}; //todo fill in graphics pipeline create info

//VkPipelineCache pipelineCache; //set to VK_NULL_HANDLE, i dont want to use it yet

//•••••••••••••••••••••••••••//
//•••create first pipeline•••//
//•••••••••••••••••••••••••••//
 VkGraphicsPipelineCreateInfo pipeline1{};  //TODO:: finish filling in struct
 pipeline1.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
 pipeline1.layout = _pipelineLayout;
 //pipeline1.pColorBlendState =
 //pipeline1.pDepthStencilState =
 pipeline1.pDynamicState = NULL;
 //pipeline1.pInputAssemblyState =
 //pipeline1.pMultisampleState =
 //pipeline1.pRasterizationState =
 //pipeline1.pStages =
 //pipeline1.pTessellationState =
 pipeline1.pVertexInputState = &inputStateCreateInfo;
 pipeline1.renderPass = _renderPass; 
 //pipeline1.stageCount =
 //pipeline1.sType =
 pipeline1.subpass = 0;
 pipelineCreateInfos.push_back(pipeline1);


 if(vkCreateGraphicsPipelines(_device.getDevice(),VK_NULL_HANDLE,static_cast<uint32_t>(pipelineCreateInfos.size()),pipelineCreateInfos.data(),nullptr,&_pipeline) != VK_SUCCESS){
    sDebug::Print("Failed to initialise graphics pipelines");
 }else{
    sDebug::Print("Successfully initialised graphics pipelines");
 }
}



sRenderer::sRenderer(sWindow& window,sDevice& device)  : _window(window), _device(device), _swapchain(device) {
    createSwapchain();
    createPipelineLayout();
    createGraphicsPipleine();

}



sRenderer::~sRenderer(){
    vkDestroyPipeline(_device.getDevice(), _pipeline,nullptr);
    vkDestroyRenderPass(_device.getDevice(),_renderPass,nullptr);
    vkDestroyPipelineLayout(_device.getDevice(),_pipelineLayout,nullptr);
    vkDestroySwapchainKHR(_device.getDevice(), _swapchain.getSwapchain(),nullptr);
}

}//namespace shb