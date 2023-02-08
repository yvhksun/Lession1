#include<iostream>

#include"vulkanext/VulkanContext.h"
#include"windows/Win32Context.h"

using namespace std;
int main(){
    
    Win32Context window;
    VulkanContext app (window);
    app.Init();
    app.loop();
    cout<<"hello"<<endl;
    return 0;
}