#include "window.hpp"
namespace shb{

void sWindow::initWindow(int height, int width, const std::string& title){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _window = glfwCreateWindow(height, width, title.c_str(), NULL, NULL);

  
}

sWindow::~sWindow(){
    glfwDestroyWindow(_window);
    glfwTerminate();
}

}//namespace shb