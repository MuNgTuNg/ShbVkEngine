#pragma once
#include <vulkan/vulkan.h>
#include "sDevice.hpp"
namespace shb{

class sBuffer{
 public:
 //creates a buffer
 //@param device device on which the buffer is created
 //@param instanceSize how big each object in the buffer is
 //@param instanceCount how many objects are in the buffer
 //@param usageFlags how the buffer is intended to be used (VERTEX_BUFFER, UNIFORM_BUFFER, TRANSFER etc)
 //@param memoryPropertyFlags memory type that is is intended to be (HOST_VISIBLE, DEVICE_LOCAL etc)
    sBuffer(sDevice& device,                          
            VkDeviceSize instanceSize,                
            uint32_t instanceCount,                   
            VkBufferUsageFlags usageFlags,            
            VkMemoryPropertyFlags memoryPropertyFlags, 
            VkDeviceSize minOffsetAlignment = 1);
        
    ~sBuffer();

  
    void map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset =0);
    void unmap();
    void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    VkBuffer& getBuffer() { return _buffer; }

 private:
    sDevice _device;

    void* _mapped = nullptr;
    VkBuffer _buffer = VK_NULL_HANDLE;
    VkDeviceMemory _memory = VK_NULL_HANDLE;

    
    VkDeviceSize _bufferSize;
    uint32_t _instanceCount;
    VkDeviceSize _instanceSize;
    VkDeviceSize _alignmentSize;
    VkBufferUsageFlags _usageFlags;
    VkMemoryPropertyFlags _memoryPropertyFlags;



    static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);


};


}//namespache shb