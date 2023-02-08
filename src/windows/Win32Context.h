#pragma once

#include<GLFW/glfw3.h>
#include<GLFW/glfw3native.h>
#include<iostream>
struct RectInfo
{
    uint32_t Height,width;
};
class Win32Context
{
private:
    /* data */
    GLFWwindow * context;
    RectInfo rect;
public:
    
    Win32Context(/* args */);
    ~Win32Context();
    template<typename T>
    void loop(T rander);
    VkSurfaceKHR CreateSurface(VkInstance instance);
    std::vector<const char*> GetExtKHR();
    HWND GetHWND();
    RectInfo GetWidthAndHeight();

};
template<typename T>
void Win32Context::loop(T rander)
{ 
    if (context)
    {
        glfwMakeContextCurrent(context);

        while (!glfwWindowShouldClose(context))
        {
            rander();
            glfwPollEvents();
        }
       
        glfwDestroyWindow(context);
        glfwTerminate();
    }else 
    {
        std::cout<< "context is null"<<std::endl;
    }
}
