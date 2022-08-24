#pragma once
//ext
#include <vulkan/vulkan.h>

//shb


#include "device.hpp"
#include "window.hpp"
//std
#include <iostream>

namespace shb{




class App
{
 public:
   //initialisation
    App();
   //normal methods
    void run();
   //destruction
    ~App();
    

 private:
   void createWindow();
   void cleanup();


   Device* _device = new Device(_window); //App outlives device

   Window* _window;
   const int HEIGHT = 1000;
   const int WIDTH = 1000;
   const std::string& TITLE = "Vulkan Window";


   
    

};

}//namespace shb