cmake_minimum_required(VERSION 3.28)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

configure_file(res/ibm_vga_fontsheet.bmp ${CMAKE_SOURCE_DIR}/build/res/ibm_vga_fontsheet.bmp COPYONLY)
configure_file(res/examplescr.json ${CMAKE_SOURCE_DIR}/build/res/examplescr.json COPYONLY)
configure_file(res/audioplayer.json ${CMAKE_SOURCE_DIR}/build/res/audioplayer.json COPYONLY)

project(conslr VERSION 1.2.2 DESCRIPTION "Simple console emulator")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(conslr SHARED
    src/console.cpp
    src/screen.cpp
    src/widgetmanager.cpp
    src/widgetfactory.cpp
)

file(GLOB conslr_headers include/conslr/*)

target_sources(conslr PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/conslr/
    FILES ${conslr_headers}
)

set_target_properties(conslr PROPERTIES CXX_STANDARD 20)
set_target_properties(conslr PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(conslr PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(conslr PRIVATE include)
target_include_directories(conslr PRIVATE ${SDL2_INCLUDE_DIR})

target_link_libraries(conslr PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(conslr PRIVATE ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(conslr PRIVATE nlohmann_json::nlohmann_json)

target_compile_options(conslr PRIVATE -Wall -Wextra -pedantic)

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
    target_link_libraries(${exampleName} PRIVATE ${CMAKE_SOURCE_DIR}/build/lib/libconslr.so)

    target_compile_options(${exampleName} PRIVATE -Wall -Wextra -pedantic)
endforeach(exampleSourceFile ${EXAMPLE_SOURCES})

include(GNUInstallDirs)

install(TARGETS conslr
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/conslr/
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/conslr/
)
