#include"VulkanContext.h"
#include<iostream>
#include"algorithm"
#include<iterator>
#include<math.h>
using namespace std;
using namespace vk;
VulkanContext::VulkanContext(/* args */Win32Context _app):win32(_app)
{
  
}

VulkanContext::~VulkanContext()
{ 
    
    
}
//  2. 初始化Vulkan实例 - 完成对vulkan库的初始化 
void VulkanContext::CreateInstance()
{

    //vector<const char*> strLayers ={"ssssssssssssss"};
    auto layers =  enumerateInstanceLayerProperties();
    for(const auto item : layers)
    {
        cout<<item.layerName<<endl;
    }
    for(const auto item : strExts)
    {
        cout<<item<<endl;
    }
  
    InstanceCreateInfo info;

    info.setPEnabledLayerNames(strLayers);
    info.setPEnabledExtensionNames(strExts);

   context = vk::createInstance(info );
}
//  4. 枚举vulkan支持的物理设备并选择满足要求的物理设备（需要查询其是否支持指定特性以及支持程度，可能存在多个vulkan支持的设备。
        //对于渲染程序，通常需要物理设备支持图形队列以及表面呈现队列）
PhysicalDevice VulkanContext::pickDevice()
{
    cout<<"pickDevice"<<endl;
    auto physicalDevs = this->context.enumeratePhysicalDevices();
    if (0 == physicalDevs.size())
    {
        throw runtime_error("No devices available!");
    }
    for(const auto pd : physicalDevs)
    {
        int i = 0;
        auto queues = pd.getQueueFamilyProperties(); 
        auto features = pd.getFeatures();
        bool ququeFlag= false,presentFlag = false;
        int queueindex,presentidnex ;


        for(auto p :queues)
        {
            //物理设备支持图形队列
            if (!ququeFlag&& p.queueFlags &vk::QueueFlagBits::eGraphics&&features.geometryShader)
            {
                ququeFlag= true;
                queueindex = i;
            }
            if(!presentFlag && pd.getSurfaceSupportKHR(i,surface))
            {
                presentFlag = true;
                presentidnex= i;
            }
            if(presentFlag&& ququeFlag)
            {
                indices.queueIndex = queueindex;
                indices.presentIndex = presentidnex;
                return pd;
            }
            i++;
        }
    }
   return physicalDevs[0];
}

vk::Device VulkanContext::CreateLogicDevice()
{
    DeviceCreateInfo info;
    DeviceQueueCreateInfo queueinfo;
    float priorities = 1.0f;
    vector<DeviceQueueCreateInfo> queues;
    queueinfo.setQueueFamilyIndex(indices.queueIndex).setPQueuePriorities(&priorities).setQueueCount(1);
    queues.push_back(queueinfo);
    if (indices.presentIndex != indices.queueIndex)
    {
       queues.push_back(DeviceQueueCreateInfo().setPQueuePriorities(&priorities).setQueueCount(1).setQueueFamilyIndex(indices.presentIndex));
    }

    const vector<const char*> deviceExtensions {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    info
    .setQueueCreateInfos(queues)
    .setPEnabledLayerNames(strLayers)
    .setPEnabledExtensionNames(deviceExtensions)
    ;
    return dev.createDevice(info);
   // this->queue =  logicdevice.getQueue(devpram.queueIndex,0);
}

void VulkanContext::CreateCommandPool()
{
    CommandPoolCreateInfo info;
    info.setQueueFamilyIndex(indices.queueIndex);
    this->commandPool =  this->logicdevice.createCommandPool(info);
}

void VulkanContext::InitWindow()
{ 
    this->strExts =win32.GetExtKHR();
    this->hWnd = win32.GetHWND();
}

void VulkanContext::clearup()
{
    if (logicdevice)
    {
        if (commandPool)
        {
            logicdevice.destroyCommandPool(commandPool);
        }
        if(swapchain)
        {
            logicdevice.destroySwapchainKHR(swapchain);
        }
        if (view.size()>0)
        {
            for (auto i : view)
            {
                logicdevice.destroyImageView(i);
            }
            view.resize(0);
        }
        
        
        logicdevice.destroy();
    }
    if (context)
    {
        if(surface){
            context.destroySurfaceKHR(surface);
        }
        context.destroy();
    }
}

void VulkanContext::CreateImageView()
{
    vector<Image> images ;
    images = logicdevice.getSwapchainImagesKHR(swapchain);
    ImageSubresourceRange range;
    range.setAspectMask(ImageAspectFlagBits::eColor) 
        .setLevelCount(1)
        .setLayerCount(1) 
        ;


    for(auto image :images)
    {
        ImageViewCreateInfo info;
        info.setImage(image)
            .setFormat(swapchaindetails.chooseFormat().format)
            .setViewType(ImageViewType::e2D)
            .setSubresourceRange(range) ;

        view.push_back(logicdevice.createImageView(info));
    }

}

void VulkanContext::CreateRenderPass()
{
    PipelineLayout layout;
}

vk::SwapchainKHR VulkanContext::CreateSwapchian()
{
    if (!this->surface)
    {
        throw runtime_error("The surface must be created befor creating swapchain!");
    }
    swapchaindetails.capabilities = dev.getSurfaceCapabilitiesKHR(surface);
    swapchaindetails.formats = dev.getSurfaceFormatsKHR(surface);
    swapchaindetails.presents = dev.getSurfacePresentModesKHR(surface);
    SwapchainCreateInfoKHR info;

    auto format = swapchaindetails.chooseFormat();
    auto extend = swapchaindetails.chooseSwapchainExtend(win32.GetWidthAndHeight());
    auto present = swapchaindetails.ChoosePresentMode();

    vector<uint32_t> indexs  = {indices.queueIndex};
    
    info.setImageSharingMode(SharingMode::eExclusive);
    if (indices.presentIndex!= indices.queueIndex)
    {
        info.setImageSharingMode(SharingMode::eConcurrent);
        indexs.push_back(indices.presentIndex);
    }
    
    info.setSurface(surface)
        .setImageColorSpace(format.colorSpace)
        .setImageFormat(format.format)
        .setImageExtent(extend)
        .setMinImageCount(2)
        .setImageUsage(ImageUsageFlagBits::eColorAttachment)
        .setPresentMode(present)
        .setImageArrayLayers(1)
        .setPreTransform(swapchaindetails.capabilities.currentTransform)
        .setQueueFamilyIndices(indexs);

    return logicdevice.createSwapchainKHR(info);
}

std::vector<const char*> VulkanContext::GetGPUNames(){
    auto gpus = context.enumeratePhysicalDevices();
    vector<const char*> result;
    for (const auto item : gpus )
    {
        auto prop = item.getProperties();
        result.push_back( prop.deviceName);
    }
    
    return result;
}

//  3. 初始化输出表面 - 指明渲染显示目的地 
void VulkanContext::CreateSurface()
{
    if (!hWnd)
    {
        throw runtime_error("Create Surface requires HWND handle!");
    }
    
    HINSTANCE instance = GetModuleHandle(nullptr);
    Win32SurfaceCreateInfoKHR info;
    info.setHinstance (instance).setHwnd(hWnd);
    surface = context.createWin32SurfaceKHR(info);
    if (this->surface)
    {
        cout<<"Win32Surface created! surface = "<<surface<<endl;
    }else 
    {
            cout<<"获取surface失败。"<<endl;
    }
}

void VulkanContext::Init()
{

   //  1. 创建程序窗口 
    this->InitWindow();

//  2. 初始化Vulkan实例 - 完成对vulkan库的初始化 
    this->CreateInstance();
//  3. 初始化输出表面 - 指明渲染显示目的地 
    this->CreateSurface();
//  4. 枚举vulkan支持的物理设备并选择满足要求的物理设备（需要查询其是否支持指定特性以及支持程度，可能存在多个vulkan支持的设备。
        //对于渲染程序，通常需要物理设备支持图形队列以及表面呈现队列）
    this->dev =  this->pickDevice();
//  5. 创建逻辑设备 - 根据指定物理设备即可创建逻辑设备
//（逻辑设备和物理设备相对应，但并不唯一。需要指出的是在创建逻辑设备时需要指定开启那些特性。
    //如果需要使用细分着色器则在这个步骤需要开启之。逻辑设备可能是vulkan中最常用的对象了，大部分vulkan对象都需要使用逻辑设备来完成创建工作）
    this->logicdevice = this->CreateLogicDevice();
//  6. 创建指令池对象 - vulkan中指令提交和传输需要通过指令缓冲来操作，对于指令缓冲需要构建指令池对象 
    this->CreateCommandPool();
//  7. 创建交换链 - vulkan中不存在默认帧缓冲的概念，需要一个缓存渲染缓冲的组件，这就是交换链。交换链本质上一个包含了若干等待呈现的图像的队列 
   if(this->surface)
   {
     this->swapchain = this->CreateSwapchian();
   }
//  8. 创建交换队列图像视图 - 有了交换链还不够，需要一组图形来保存渲染数据 
    this->CreateImageView();
//  9. 创建渲染通道 - 渲染通道用于表示帧缓冲，他是子通道以及子通道关系的集合。深度模板附件、颜色附件、帧附件都是在此阶段被创建的 
        //9.1 创建


    this->CreateRenderPass();
//  10. 创建描述符设置布局 - 描述符是一个特殊的不透明的着色器变量，着色器使用它以间接的方式访问缓冲区和图像资源。描述符集合是描述一个管线使用到的描述符集合。描述符布局则用于刻画其布局。 
//  11. 创建管线布局 - 管道布局包含一个描述符集合布局的列表。推送常量在这个阶段被设置。 
//  12. 创建帧缓冲 - 作为附件的图像依赖交换链用于呈现时返回的图像。这意味着我们必须为交换链中的所有图像创建一个帧缓冲区，并在绘制的时候使用对应的图像。其附件必须和渲染通道中使用的附件相匹配。 
//  13. 创建描述符池 -描述符不能直接创建，需要从池中分配 
//  14. 创建描述符集 - 描述符池是根据交换链帧个数以及具体Shader中描述符数量和数据来创建的，着色器属性类型和其中涉及的uninform、location等信息是在这个阶段被传入的 
//  15. 分配和更行描述符集 - 根据交换链帧个数、uniform数据以及图形视图是在这个阶段被处理的 
//  16. 创建管线 - 根据Shader、管线布局、渲染通道以及其他相关信息即可构造管线 
//  17. 创建命令缓冲 - 根据命令池、渲染通道、交换链帧个数即可分配并使用命令缓冲，其中对管线的绑定、描述符集的绑定以及开始和结束渲染通道是在这个阶段完成的。从某种意义上讲，这块类似早期OpenGL中的显示列表，决定了最终的渲染效果



}

void VulkanContext::loop()
{ 
   // cout<<"遍历循环"<<endl;
    win32.loop([this]()->void{
        
    });

    clearup();
}

//交换模式
vk::PresentModeKHR VulkanContext::SwapchainDetails::ChoosePresentMode()
{
    vk::PresentModeKHR result = vk::PresentModeKHR ::eFifo;
    for(const auto present: presents)
    {
     //   present.
        if (vk::PresentModeKHR::eMailbox == present)
            return present;
    }
    return result;
}

//色彩空间
vk::SurfaceFormatKHR VulkanContext::SwapchainDetails::chooseFormat()
{
   for(const auto format : formats)
   {
        if (format.format == vk::Format::eR8G8B8A8Unorm&& format.colorSpace == ColorSpaceKHR::eSrgbNonlinear )
        {
            return format;
        }
   }
   return { vk::Format::eR8G8B8A8Unorm,ColorSpaceKHR::eSrgbNonlinear};
}

//交换区域
vk::Extent2D VulkanContext::SwapchainDetails::chooseSwapchainExtend(RectInfo rect)
{
    VkExtent2D  result = {rect.width,rect.Height};
    result.height = max(capabilities.minImageExtent.height,min(result.height,capabilities.maxImageExtent.height));
    result.width = max(capabilities.minImageExtent.width,min(result.width,capabilities.maxImageExtent.width));
    return result;
}
