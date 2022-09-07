#include "app.hpp"
#include "sWindow.hpp"



namespace shb{


void App::run(){
    while(!glfwWindowShouldClose(_window.getWindow())){
        glfwWaitEvents();
    }
}


//////////////////////initialisation
App::App(){
  
  
}








///////////////////destruction
void App::cleanup()
{


}
App::~App(){
    cleanup();
}

}//namespace shb