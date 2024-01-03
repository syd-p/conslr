cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(conslr)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(conslr SHARED
    src/console.cpp
)

set_target_properties(conslr PROPERTIES CXX_STANDARD 20)
set_target_properties(conslr PROPERTIES PUBLIC_HEADER include/conslr/consle.hpp)

target_include_directories(conslr PRIVATE include)
target_include_directories(conslr PRIVATE src)
target_include_directories(conslr PRIVATE ${SDL2_INCLUDE_DIR})

target_link_libraries(conslr PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(conslr PRIVATE ${SDL2_IMAGE_LIBRARIES})

project(test)

add_executable(test tests/main.cpp)

set_target_properties(test PROPERTIES CXX_STANDARD 20)

target_include_directories(test PRIVATE ${SDL2_INCLUDE_DIR})
target_include_directories(test PRIVATE include)

target_link_libraries(test PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(test PRIVATE ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(test PRIVATE ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/libconslr.so)
