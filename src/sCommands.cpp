#include "sCommands.hpp"

namespace shb{

sCommands::sCommands(sDevice& d, sSwapchain& s, VkPipeline& p) : _device(d), _swapchain(s), _pipeline(p){
    createCommandPool();
    allocateCommandBuffers();
}


void sCommands::oneTimeCommand(){
    beginCommandBuffer(cmdBufferIndex);
    recordCommandBuffer(cmdBufferIndex);
    endCommandBuffer(cmdBufferIndex);
}



void sCommands::beginCommandBuffer(int index){
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if(vkBeginCommandBuffer(_commandBuffers[index],&beginInfo)!= VK_SUCCESS){
        sDebug::Print("Failed to start recording command buffer");
    }else{
        sDebug::Print("Recording command buffer...");
    }
}



void sCommands::recordCommandBuffer(int index){
    
    std::vector<VkClearValue> clearValues{};

    VkClearValue clearValue1{};
    clearValue1.color = {1,33,223,111};
    clearValues.push_back(clearValue1);

    VkClearValue clearValue2{};
    clearValue2.color = {1,33,223,111};
    clearValues.push_back(clearValue2);
    
    
    VkRenderPassBeginInfo beginInfo{};
    beginInfo.clearValueCount = clearValues.size();
    beginInfo.pClearValues = clearValues.data();
    beginInfo.framebuffer = _swapchain.getFrameBuffer(index);
    beginInfo.renderArea = {1000,1500};
    beginInfo.renderPass = _renderPass;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    
   sVertex vertices[3];
   for(int i =0; i<=2; ++i){

   
    vertices[i] = {
        glm::vec3{130.f,231.f,131.f},
        glm::vec3{244.f,234.f,234.f}
    };
   }

   VkBuffer vBuffer;
   
    

    vkCmdBeginRenderPass(_commandBuffers[index],&beginInfo,VK_SUBPASS_CONTENTS_INLINE);
    
    vkCmdBindVertexBuffers(_commandBuffers[index],0,sizeof(vBuffer),vBuffer,0);
    vkCmdBindPipeline(_commandBuffers[index],VK_PIPELINE_BIND_POINT_GRAPHICS,_pipeline);
    vkCmdDraw(_commandBuffers[index],3,0,1,0);

    vkCmdEndRenderPass(_commandBuffers[index]);
    
}



void sCommands::endCommandBuffer(int index){
    if(vkEndCommandBuffer(_commandBuffers[index]) != VK_SUCCESS){
        sDebug::Print("Failed to end command buffer");
    }else{
        sDebug::Print("Successfully recorded command buffer...");
    }
}



// void sCommands::submitCommandBuffer(int index){

//     VkSubmitInfo cmdSubmitInfo{};
//     cmdSubmitInfo.commandBufferCount = _commandBuffers.size();
//     cmdSubmitInfo.pCommandBuffers = _commandBuffers.data();
//     cmdSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//     cmdSubmitInfo.

//     vkQueueSubmit(_device.getGraphicsQueue(),&cmdSubmitInfo,_fence)
// }



void sCommands::createCommandPool(){
    QueueFamilyIndices QFI = _device.findQueueFamilies(_device.getPhysicalDevice());
    
    VkCommandPoolCreateInfo createInfo{};
    createInfo.queueFamilyIndex = QFI.graphicsFamily;
    createInfo.pNext = NULL;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if(vkCreateCommandPool(_device.getDevice(),&createInfo,nullptr,&_commandPool)!= VK_SUCCESS){
        sDebug::Print("Failed to create command pool");
    }else{
        sDebug::Print("Successfully created command pool");
    }
}



void sCommands::allocateCommandBuffers(){
    _commandBuffers.push_back(_commandBuffer1);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandBufferCount = _commandBuffers.size();
    allocInfo.commandPool = _commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.pNext = NULL;

    if(vkAllocateCommandBuffers(_device.getDevice(),&allocInfo,_commandBuffers.data())!=VK_SUCCESS){
        sDebug::Print("Failed to allocate Command buffers");
    }else{
        sDebug::Print("Successfully allocated Command buffers");
    }
}

void sCommands::recieveRenderPass(VkRenderPass rp){
    if(rp == VK_NULL_HANDLE){
        sDebug::Print("Render pass lost in translation at recieveRenderPass ");
    }
    _renderPass = rp;
}

sCommands::~sCommands(){
    vkDestroyCommandPool(_device.getDevice(),_commandPool,nullptr);

}


}//namespace shb