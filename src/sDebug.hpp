#pragma once 
#include <string>
#include <iostream>

namespace shb{
    static int _count = 1;

    class sDebug{
     public:
        static void Print(const std::string& msg){
             
             std::cout << _count << ": " << msg << std::endl;
             ++_count;
        }

    };

    

}//namespace shb