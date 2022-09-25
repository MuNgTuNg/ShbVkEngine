

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
 
 
  //get physical device properties to check for input bindings and limits settings
    VkPhysicalDeviceProperties deviceProperties{};
    vkGetPhysicalDeviceProperties(_device.getPhysicalDevice(),&deviceProperties);


  //binding description is like an overview of the data you're feeding to the shader
  //binding is the kind of group of information you're describing and stride
  //is the size of each input, each input is further segmented and described
  //in the attribute descriptions, e.g the size of each individual component
    VkVertexInputBindingDescription inputBindingDescription1{};
    inputBindingDescription1.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    uint32_t binding = 0;              //index 0 is one sort of group of objects(?)
    uint32_t stride = sizeof(sVertex); //stride is the total size of each individual vertex being fed to the shader program
    
    if(binding <= deviceProperties.limits.maxVertexInputBindings 
       && stride <= deviceProperties.limits.maxVertexInputBindingStride){
        inputBindingDescription1.binding = binding; 
        inputBindingDescription1.stride = stride;

    }

    inputBindingDescriptions.push_back(inputBindingDescription1);
   

  //attribute descriptions describe the component parts of a vertex, VK_FORMAT_R32_G32_SFLOAT describes two floats, used as 2d coordinates x and y
    VkVertexInputAttributeDescription xyAttribute{};
    xyAttribute.format = VK_FORMAT_R32G32B32_SFLOAT; //confusingly this uses the same enum as the colorspace
    
  //basic checking if the offset is within limits
    uint32_t xyAttrOffset = 0;
    uint32_t xyAttrLocation = 0;
  
    if(xyAttrOffset <= deviceProperties.limits.maxVertexInputAttributeOffset 
       && inputBindingDescription1.binding <= deviceProperties.limits.maxVertexInputBindings   
       && xyAttrLocation <= deviceProperties.limits.maxVertexInputAttributes ){

        xyAttribute.offset = xyAttrOffset;         //where the data for this component starts within the struct e.g after 2 floats (2*sizeof(float))
        xyAttribute.binding = inputBindingDescription1.binding;       //binding is what other attributes it is bound to in a kind of group
        xyAttribute.location = xyAttrLocation;    //location corresponds with the GLSL (location )  

    }else{
        sDebug::Print("Attribute descriptions unsuitable");
    }
    attributeDescriptions.push_back(xyAttribute);
  
  //color component of my vertex = 3 integers
    VkVertexInputAttributeDescription colorAttribute{};
    colorAttribute.format = VK_FORMAT_R32G32B32_UINT;

    //basic checking if the offset is within limits
    uint32_t colorAttrOffset = 3*sizeof(float); //after x,y,z position
    uint32_t colorAttrLocation = 1;             //location 1
    if(colorAttrOffset <= deviceProperties.limits.maxVertexInputAttributeOffset 
       && inputBindingDescription1.binding <= deviceProperties.limits.maxVertexInputBindings   
       && colorAttrLocation <= deviceProperties.limits.maxVertexInputAttributes ){

        colorAttribute.offset = colorAttrOffset;                          //where the data for this component starts within the struct e.g after 2 floats (2*sizeof(float))
        colorAttribute.binding = inputBindingDescription1.binding;       //binding is what other attributes it is bound to in a kind of group
        colorAttribute.location = colorAttrLocation;                     //location corresponds with the GLSL (location )  

    }else{
        sDebug::Print("Attribute descriptions unsuitable");
    }
    attributeDescriptions.push_back(colorAttribute);
  

    
   
  //used to create the graphics pipeline
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


 //set rasterizer state
 VkPipelineRasterizationStateCreateInfo rasterInfo{}; //todo
 rasterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;


//do the necessary things to create a renderpass
 createRenderPass();


//create vectors necessary for the inputStateCreateInfos to point to, if you don't pass them in they get deleted and inputstateCreateInfo points to nothing
 std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
 std::vector<VkVertexInputBindingDescription> inputBindingDescriptions{};
 VkPipelineVertexInputStateCreateInfo inputStateCreateInfo {};

//make all necessary settings to structs in order to decide how vertices are inputted to the shader programme
 setVertexInput(attributeDescriptions,inputBindingDescriptions,inputStateCreateInfo); //TODO:: abstract this into a static function in a vertex class, it belongs to vertex


// "draw triangles please"
 VkPipelineInputAssemblyStateCreateInfo assemblyStateCreateInfo{};
 assemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
 assemblyStateCreateInfo.pNext = NULL;
 assemblyStateCreateInfo.flags = 0;
 assemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;


 //signal that the states of the viewport and scissor will change over time
 std::vector<VkDynamicState> dynamicStates = {
     VK_DYNAMIC_STATE_VIEWPORT,
     VK_DYNAMIC_STATE_SCISSOR
 };
 
 VkPipelineDynamicStateCreateInfo dynamicState{};
 dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
 dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
 dynamicState.pDynamicStates = dynamicStates.data();

//set scissor to window height and width
 VkRect2D scissor{};
 scissor.extent.height = _window._height;
 scissor.extent.width = _window._width;

//set viewport to match window height and width also
 VkViewport viewport{};
 viewport.height = _window._height;
 viewport.width = _window._width;
 viewport.x = 0.0f;                   //x = far left of screen
 viewport.y = 0.0f;                   //y = top of screen
 viewport.minDepth = 0.0f;            //minimum depth check
 viewport.maxDepth = 1.0f;            //maximum depth check

//create info relating to viewport and scissor
 VkPipelineViewportStateCreateInfo viewportCreateInfo{};
 viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
 viewportCreateInfo.pScissors = &scissor;
 viewportCreateInfo.pViewports = &viewport;
 viewportCreateInfo.scissorCount = 1;
 viewportCreateInfo.viewportCount = 1;


 std::vector<VkPipelineShaderStageCreateInfo> shaderStages{};

 VkShaderModule vertexShaderCode{}; //= createShaderModule("/shaders/vertShader.vert.spv")
 VkShaderModule fragmentShaderCode{}; //= createShaderModule("/shaders/fragShader.frag.spv")

 VkPipelineShaderStageCreateInfo vertexShader{};
 vertexShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
 vertexShader.pNext = NULL;
 vertexShader.stage = VK_SHADER_STAGE_VERTEX_BIT;
 vertexShader.pName = "main";
 vertexShader.module = vertexShaderCode;

 shaderStages.push_back(vertexShader);

 VkPipelineShaderStageCreateInfo fragmentShader{};
 fragmentShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
 fragmentShader.pNext = NULL;
 fragmentShader.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
 fragmentShader.pName = "main";
 fragmentShader.module = fragmentShaderCode;

 shaderStages.push_back(fragmentShader);


//fill in information about pipelines wanting to be created
 std::vector<VkGraphicsPipelineCreateInfo> pipelineCreateInfos{}; 

//VkPipelineCache pipelineCache; //set to VK_NULL_HANDLE, i dont want to use it yet

//•••••••••••••••••••••••••••//
//•••create first pipeline•••//
//•••••••••••••••••••••••••••//
 VkGraphicsPipelineCreateInfo pipeline1{};  //TODO:: finish filling in struct
 pipeline1.sType =                     VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
 pipeline1.layout =                    _pipelineLayout;
 //pipeline1.pColorBlendState =
 //pipeline1.pDepthStencilState =
 pipeline1.pViewportState =            &viewportCreateInfo;
 pipeline1.pDynamicState =             &dynamicState;
 pipeline1.pInputAssemblyState =       &assemblyStateCreateInfo;
 //pipeline1.pMultisampleState =
 //pipeline1.pRasterizationState =     &rasterInfo;
 pipeline1.pStages =                   shaderStages.data();
 pipeline1.stageCount =                static_cast<uint32_t>(shaderStages.size());
 //pipeline1.pTessellationState =
 pipeline1.pVertexInputState =         &inputStateCreateInfo;
 pipeline1.renderPass =                _renderPass; 
 pipeline1.subpass =                   0;
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