#include "app.hpp"
#include "sWindow.hpp"



namespace shb{


void App::run(){
    while(!glfwWindowShouldClose(_window.getWindow())){
        int i = 0;
        while(i <1){
            _renderer.render();
            ++i;
        }
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