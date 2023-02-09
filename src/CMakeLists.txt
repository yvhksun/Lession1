


add_executable(myapp "Main.cpp")
# target_include_directories(myapp PUBLIC "C:\\VulkanSDK\\1.3.204.0\\Include")
# target_link_directories(myapp PUBLIC "C:\\VulkanSDK\\1.3.204.0\\Lib")

target_link_libraries(myapp PUBLIC vulext win32cxt)


set_target_properties(myapp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)