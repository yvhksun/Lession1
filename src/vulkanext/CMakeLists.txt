

aux_source_directory(. cppfiles)
message( ${cppfiles})

add_library(vulext SHARED ${cppfiles})



target_link_directories(vulext PRIVATE "${vulkandir}/Lib")
target_link_libraries(vulext PRIVATE "vulkan-1.lib" win32cxt)
set_target_properties(vulext PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)