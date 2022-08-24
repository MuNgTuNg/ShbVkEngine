
#include "window.hpp"
#include <string>

Window::Window(int width, int height, const std::string& title) : _width(width), _height(height), _title(title){
  
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    
}
Window::~Window(){
    glfwDestroyWindow(_window);
    glfwTerminate();
}



void Window::changeWindowSize(int width, int height, Window& window) 
{
   
}