cmake_minimum_required (VERSION 3.22)
aux_source_directory(. cppFiles)

add_library(win32cxt SHARED ${cppFiles})

target_link_directories(win32cxt PRIVATE "C:\\VulkanSDK\\1.3.204.0\\Third-Party\\glfw\\lib-mingw-w64")
target_link_libraries(win32cxt PRIVATE "glfw3.dll")

set_target_properties(win32cxt PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)