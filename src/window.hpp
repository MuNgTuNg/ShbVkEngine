#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <assert.h>
#include <string>

class Window{

     public:
     Window(int width, int height, const std::string& title);
     ~Window();
     Window operator=(const Window&) = delete;
     Window(const Window&) = delete;

     GLFWwindow* getwindow() { 
        assert(_window!=nullptr && "window* is nullptr");
        return _window; 
        };
     static void changeWindowSize(int width, int height,Window& window);



    private:
     

     GLFWwindow* _window;

     int _width;
     int _height;
     std::string _title;
     

};