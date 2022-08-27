#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string> 

namespace shb{
class sWindow{
 public:

   sWindow() = default;
   sWindow(int width, int height, const std::string& title) {
        initWindow(width,height,title);
   }
   ~sWindow();
   GLFWwindow* getWindow(){
    return _window;
   }


 private:
   void initWindow(int width, int height, const std::string& title);

   int _width;
   int _height;
   std::string title;
   GLFWwindow* _window;

};

}//namespace shb