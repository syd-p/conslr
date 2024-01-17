cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

configure_file(res/ibm_vga_fontsheet.bmp ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/bin/ibm_vga_fontsheet.bmp COPYONLY)
configure_file(include/conslr/conslr.hpp ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/include/conslr.hpp COPYONLY)
file(GLOB WIDGET_HEADERS include/conslr/widgets/*.hpp)
foreach(widgetHeader ${WIDGET_HEADERS})
    get_filename_component(widgetHeaderName ${widgetHeader} NAME)
    configure_file(${widgetHeader} ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/include/widgets/${widgetHeaderName} COPYONLY)
endforeach(widgetHeader ${WIDGET_HEADERS})

project(conslr VERSION 1.1.1 DESCRIPTION "Simple console emulator")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(conslr SHARED
    src/console.cpp
    src/screen.cpp
    src/widgetmanager.cpp
)

set_target_properties(conslr PROPERTIES CXX_STANDARD 20)
set_target_properties(conslr PROPERTIES PUBLIC_HEADER include/conslr/consle.hpp)
set_target_properties(conslr PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(conslr PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(conslr PRIVATE include)
target_include_directories(conslr PRIVATE src)
target_include_directories(conslr PRIVATE ${SDL2_INCLUDE_DIR})

target_link_libraries(conslr PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(conslr PRIVATE ${SDL2_IMAGE_LIBRARIES})

#examples

file(GLOB EXAMPLE_SOURCES examples/*.cpp)
foreach(exampleSourceFile ${EXAMPLE_SOURCES})
    get_filename_component(exampleName ${exampleSourceFile} NAME_WE)
    add_executable(${exampleName} ${exampleSourceFile})

    set_target_properties(${exampleName} PROPERTIES CXX_STANDARD 20)

    target_include_directories(${exampleName} PRIVATE ${SDL2_INCLUDE_DIR})
    target_include_directories(${exampleName} PRIVATE include)

    target_link_libraries(${exampleName} PRIVATE ${SDL2_LIBRARIES})
    target_link_libraries(${exampleName} PRIVATE ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries(${exampleName} PRIVATE ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib/libconslr.so)
endforeach(exampleSourceFile ${EXAMPLE_SOURCES})
