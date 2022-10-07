#pragma once
#include <vulkan/vulkan.h>
#include "sDevice.hpp"
#include "sBuffer.hpp"
#include "sSwapchain.hpp"
#include "sObjects.hpp"



// · a class that holds all things to do with commands in my engine
//      »input from keyboard and mouse
//      »recording and submitting command buffers
namespace shb{



class sCommands{
 public:
    sCommands(sDevice& d, sSwapchain& s, VkPipeline& p,VkRenderPass& r);
    ~sCommands();
    
    void createCommandPool();
    void allocateCommandBuffers();

    void oneTimeCommand();

    void beginCommandBuffer(int currentBuffer);
    void recordCommandBuffer(int currentBuffer);
    void endCommandBuffer(int currentBuffer);
    void submitCommandBuffer(int currentBuffer);



 private:

    sVBO vbo;
    
    int cmdBufferIndex =0;
    std::vector<VkCommandBuffer> _commandBuffers{};
    VkCommandBuffer _commandBuffer1;

    VkCommandPool _commandPool;
    VkFence _fence;
    
    VkRenderPass& _renderPass;
    sDevice& _device;
    sSwapchain& _swapchain;
    VkPipeline& _pipeline;
    
    void recHandW(int h, int w) { HEIGHT = h; WIDTH = w;}
    int HEIGHT;
    int WIDTH;


};



}//namespace shb