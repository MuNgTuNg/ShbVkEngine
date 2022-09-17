#include "sWindow.hpp"
#include <iostream>
namespace shb{

void sWindow::initWindow(int height, int width, const std::string& title){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window = glfwCreateWindow(height, width, title.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(_window, this);


  
}

void sWindow::createSurface(VkInstance _instance, VkSurfaceKHR* _surface) {

     if(glfwCreateWindowSurface(_instance,_window,nullptr,_surface)!= VK_SUCCESS){
      sDebug::Print("Surface creation failed");
    }else{
      sDebug::Print("Surface creation success");
    }
  }

sWindow::~sWindow(){
    glfwDestroyWindow(_window);
    glfwTerminate();
}

}//namespace shb