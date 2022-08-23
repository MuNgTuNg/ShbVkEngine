#pragma once
#include <iostream>



// namespace shb{
// //need to make more interesting
    


class ShbDebug{
 public:
    ShbDebug() = default;
    const bool DEBUG = true;
    void log(const std::string& msg){
        if(DEBUG == true){
            std::cout<<msg << "\n";
        }
    }
    void log (int i){
        if(DEBUG == true){
	   std::cout<<i << "\n";
	    }
    }
    
   
    
};

// }//namespace shb
