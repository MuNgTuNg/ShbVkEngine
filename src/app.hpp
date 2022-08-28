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
   void cleanup();

   
   const int HEIGHT = 1000;
   const int WIDTH = 1000;
   const std::string& TITLE = "Vulkan Window";
   sWindow _window{HEIGHT,WIDTH,TITLE};

   sDevice _device{_window};


   
    

};

}//namespace shb