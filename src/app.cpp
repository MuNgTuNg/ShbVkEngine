#include "app.hpp"
#include "window.hpp"



namespace shb{


void App::run(){
    while(!glfwWindowShouldClose(_window->getWindow())){
        glfwWaitEvents();
    }
}


//////////////////////initialisation
App::App(){
  createWindow();
  
}


void App::createWindow(){
    _window = new sWindow(HEIGHT,WIDTH,TITLE);
}





///////////////////destruction
void App::cleanup()
{
    _window = nullptr;
    //_device = nullptr;

}
App::~App(){
    cleanup();
}

}//namespace shb