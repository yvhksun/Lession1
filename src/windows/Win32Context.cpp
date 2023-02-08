
#include<vulkan/vulkan.hpp>
#include"Win32Context.h"

using namespace std;
Win32Context::Win32Context(/* args */)
{    
    cout<< "Iinit Window"<<endl;
    glfwInit();
    // glfwInitHint(GLFW_CLIENT_API,GLFW_NO_API);
    // glfwInitHint(GLFW_RESIZABLE,GLFW_FALSE);
    rect = {750,1200};
    context = glfwCreateWindow(rect.width,rect.Height,"vulkantest",nullptr,nullptr);
}

Win32Context::~Win32Context()
{
}


VkSurfaceKHR Win32Context::CreateSurface(VkInstance instance)
{
    cout<<"create Surface"<<endl;
    if (!context)
    {
        throw std::runtime_error("The winows handle must be created befor creating surface!");
    }
    if (!instance)
    {
        throw runtime_error("VkInstance is null!");
    }

    VkSurfaceKHR surface ;
    cout<<"instance:"<<instance<<"\t context:"<<context<<endl;
    VkResult result =  glfwCreateWindowSurface(instance,context,nullptr,&surface);

    if(VK_SUCCESS !=  result){

        cout<< "create surface feild!"<<result<<endl;
        throw std::runtime_error("failed to create window surface! ");
    }else{
        cout<< "create surface success!"<<surface<<endl;
    }
    return surface;
}

vector<const char*>  Win32Context::GetExtKHR()
{
    uint32_t len =0;
    auto temp = glfwGetRequiredInstanceExtensions(&len);
    vector<const char*> result (temp,temp+len);
    return result;
}

HWND Win32Context::GetHWND()
{
    return glfwGetWin32Window(context);
}

RectInfo Win32Context::GetWidthAndHeight()
{
    return rect;
}
