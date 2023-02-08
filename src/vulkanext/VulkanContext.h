#pragma once
#include<iostream>
#include<vulkan/vulkan.hpp>
#include<GLFW/glfw3.h>
#include"../windows/Win32Context.h"
class VulkanContext
{
private:
    /* data */
    vk::Instance context;
    vk::SurfaceKHR surface;
    vk::PhysicalDevice dev;
    vk::Device logicdevice;
    vk::Queue queue;
    vk::SwapchainKHR swapchain;
    vk::CommandPool commandPool;
    std::vector<vk::ImageView> view;
    std::vector<const char*> strExts = {"VK_KHR_surface","VK_KHR_win32_surface"};
    std::vector<const char*> strLayers = {"VK_LAYER_KHRONOS_validation"};

    Win32Context win32;
     

    HWND hWnd;

    
    struct QueueFamilyIndices
    {
        uint32_t queueIndex;
        uint32_t presentIndex;
    };
    struct SwapchainDetails{
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presents;

        vk::PresentModeKHR ChoosePresentMode();
        vk::SurfaceFormatKHR chooseFormat();
        vk::Extent2D chooseSwapchainExtend(RectInfo rect);
    };
    QueueFamilyIndices indices;
    SwapchainDetails swapchaindetails;
public:
    VulkanContext(/* args */Win32Context _app);
    ~VulkanContext();

    std::vector<const char*> GetGPUNames();
    void Init();
    void loop();
private:
    void CreateSurface(); 
    void CreateInstance();  
    void CreateCommandPool();
    void InitWindow();

    void clearup();
    void CreateImageView();

protected:
    vk::PhysicalDevice pickDevice();
    vk::Device CreateLogicDevice();
    vk::SwapchainKHR CreateSwapchian();
};

