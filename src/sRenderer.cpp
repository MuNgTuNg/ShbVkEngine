

#include "sRenderer.h"

namespace shb{

void sRenderer::createSwapchain(){
    _swapchain.createSwapchain();

}

sRenderer::sRenderer(sWindow& window,sDevice& device)  : _window(window), _device(device), _swapchain(device) {
    createSwapchain();
}

sRenderer::~sRenderer(){
    vkDestroySwapchainKHR(_device.getDevice(), _swapchain.getSwapchain(),nullptr);
}

}//namespace shb