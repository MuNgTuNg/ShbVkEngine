

#include "sRenderer.hpp"

namespace shb{

void sRenderer::createSwapchain(){
    _swapchain.createSwapchain();
    _swapchain.createImageViews();

}

sRenderer::sRenderer(sWindow& window,sDevice& device)  : _window(window), _device(device), _swapchain(device) {
    createSwapchain();
    
}

sRenderer::~sRenderer(){
    
    vkDestroySwapchainKHR(_device.getDevice(), _swapchain.getSwapchain(),nullptr);
}

}//namespace shb