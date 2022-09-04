#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string> 

#include "sSwapchain.h"

namespace shb{
class sWindow{
 public:
   friend class sDevice;

   sWindow() = default;
   sWindow(int width, int height, const std::string& title) : _width(width), _height(height), _title(title){
        initWindow(width,height,title);
   }
   ~sWindow();
   GLFWwindow* getWindow(){
    return _window;
   }


 private:
   void initWindow(int width, int height, const std::string& title);
   void createSurface(VkInstance,VkSurfaceKHR*);

   int _width;
   int _height;
   std::string _title;
   GLFWwindow* _window;
   VkSurfaceKHR _surface;
   

};

}//namespace shb