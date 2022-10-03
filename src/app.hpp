#pragma once
//ext
#include <vulkan/vulkan.h>

//shb


#include "sDevice.hpp"
#include "sWindow.hpp"
#include "sRenderer.hpp"
//std
#include <iostream>
 
 #define HEIGHT 1000
 #define WIDTH 1500

//file search keys:
//     (???) source of confusion that needs to be addressed (possible future errors)
//     TODO:: stuff that needs to be done 
//     (research) stuff that needs to be researched


//todo add debug messages when all things are destroyed

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

   
   const std::string& TITLE = "Vulkan Window";
   sWindow _window{HEIGHT,WIDTH,TITLE};

   sDevice _device{_window};
   sRenderer _renderer{_window, _device};

  

};

}//namespace shb