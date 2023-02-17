cmake_minimum_required (VERSION 3.18)
project("myapp")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

 add_definitions(-DVK_VERSION_1_0 -DGLFW_EXPOSE_NATIVE_WIN32 -DVK_USE_PLATFORM_WIN32_KHR) 

 #set(vk204dir "C:/VulkanSDK/1.3.204.0")

 message($ENV{VULKAN_SDK})

 set(vulkandir $ENV{VULKAN_SDK})
 set(glfwdir "${vulkandir}/Third-Party/glfw")
#  find_package(Vulkan REQUIRED)
#  message(${Vulkan_LIBRARY})
#  message(${Vulkan_INCLUDE_DIRS})
include_directories(
    "${vulkandir}/Include"
    "${glfwdir}/include"
    )

add_subdirectory(src)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK")
add_subdirectory(src/vulkanext)
add_subdirectory(src/windows)

    #  message(${CMAKE_SIZEOF_VOID_P})
 
# "把DEBUG版的可执行Target放到${CMAKE_BINARY_DIR}/bin目录中"
# set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
 
# "把RELEASE版的可执行Target放到${CMAKE_BINARY_DIR}/bin目录中"
# set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
