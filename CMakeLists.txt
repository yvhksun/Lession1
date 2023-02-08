cmake_minimum_required (VERSION 3.22)
project("myapp")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

 add_definitions(-DVK_VERSION_1_0 -DGLFW_EXPOSE_NATIVE_WIN32 -DVK_USE_PLATFORM_WIN32_KHR) 

include_directories(
    "C:\\VulkanSDK\\1.3.204.0\\Include"
    "C:\\VulkanSDK\\1.3.204.0\\Third-Party\\glfw\\include"
    )

add_subdirectory(src)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK")
add_subdirectory(src/vulkanext)
add_subdirectory(src/windows)



 
# "把DEBUG版的可执行Target放到${CMAKE_BINARY_DIR}/bin目录中"
# set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
 
# "把RELEASE版的可执行Target放到${CMAKE_BINARY_DIR}/bin目录中"
# set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
