#include "sBuffer.hpp"

namespace shb{


//@param instanceSize size of each object in buffer
//@param minOffsetAlignment not sure, look up
VkDeviceSize sBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment = 1){
    //if the minimum offset alignment is larger than zero
     if (minOffsetAlignment > 0) {
    return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1); //no idea whatsoever? todo
  }
  return instanceSize;
}


//constructor takes all parameters of buffer creation
sBuffer::sBuffer(sDevice& device,
                           VkDeviceSize instanceSize,
                           uint32_t instanceCount,
                           VkBufferUsageFlags usageFlags,
                           VkMemoryPropertyFlags memoryPropertyFlags,
                           VkDeviceSize minOffsetAlignment) : _device(device),
                           _instanceSize(instanceSize),
                           _instanceCount(instanceCount),
                           _usageFlags(usageFlags),
                           _memoryPropertyFlags(memoryPropertyFlags)
{

_alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
_bufferSize = _alignmentSize * instanceCount;

_device.createBuffer(_buffer,_bufferSize,_usageFlags,_memory,_memoryPropertyFlags);
  
}

sBuffer::~sBuffer(){
    unmap();
    vkDestroyBuffer(_device.getDevice(),_buffer,nullptr);
    vkFreeMemory(_device.getDevice(),_memory,nullptr);
}

//@param size the size of the buffer we want to map
//@param offset the offset of the buffer in the mapping
void sBuffer::map(VkDeviceSize size, VkDeviceSize offset){
  //map the _memory vkDeviceMemory todo research
    vkMapMemory(_device.getDevice(),_memory,offset,size,_memoryPropertyFlags,&_mapped);
}

void sBuffer::unmap() {
  if (_mapped) {
    vkUnmapMemory(_device.getDevice(), _memory);
    _mapped = nullptr;
  }
}

//@param data is the raw array of data to copy into the mapped memory range
void sBuffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset){
    if(size == VK_WHOLE_SIZE){            //if we want to copy the whole buffer
        memcpy(_mapped,data,_bufferSize); //copy the range passed in when creating sBuffer object
    }else{
        char *memOffset = (char *)_mapped; //else add the offset value to the start of the mapped data, then copy
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}





}//namespace shb