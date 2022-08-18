#include "app.hpp"
#include "window.hpp"



namespace shb{


void App::run(){
    while(!glfwWindowShouldClose(_window->getwindow())){
        glfwWaitEvents();
    }
}


//////////////////////initialisation
App::App(){
  createWindow();
  
}


void App::createWindow(){
    _window = new Window(HEIGHT,WIDTH,TITLE);
}





///////////////////destruction
void App::cleanup()
{
    _window = nullptr;
    _device = nullptr;

}
App::~App(){
    cleanup();
}

}//namespace shb