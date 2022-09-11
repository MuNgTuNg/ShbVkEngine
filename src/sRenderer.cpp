

#include "sRenderer.hpp"

namespace shb{

void sRenderer::createSwapchain(){
    _swapchain.createSwapchain();
    _swapchain.createImageViews();

}


void sRenderer::createGraphicsPipleine(){
 
 //VkPipelineCache pipelineCache; //set to VK_NULL_HANDLE, i dont want to use it yet
 
 
 std::vector<VkGraphicsPipelineCreateInfo> createInfo{}; //todo fill in graphics pipeline create info

 if(vkCreateGraphicsPipelines(_device.getDevice(),VK_NULL_HANDLE,static_cast<uint32_t>(createInfo.size()),createInfo.data(),nullptr,&_pipeline) != VK_SUCCESS){
    std::cout<<"Failed to initialise graphics pipelines\n";
 }else{
    std::cout<<"Successfully initialised graphics pipelines\n";
 }
}


sRenderer::sRenderer(sWindow& window,sDevice& device)  : _window(window), _device(device), _swapchain(device) {
    createSwapchain();
    
}

sRenderer::~sRenderer(){
    
    vkDestroySwapchainKHR(_device.getDevice(), _swapchain.getSwapchain(),nullptr);
}

}//namespace shb