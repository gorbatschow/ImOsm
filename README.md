# ImOsmWidget
This widget renders OpenStreetMap tiles in Dear ImGui window.

It is quite possible to use any other tile provider even local, just change the URL.

## In action
Look at the ![ImOsmWidgetDemo](https://github.com/gorbatschow/ImOsmWidgetDemo).

![image](https://github.com/gorbatschow/ImOsmWidgetDemo/blob/master/doc/screenshot.png?raw=true)

## CMake example
This repo doesn't contain any build system files, but here's CMake example.
~~~
cmake_minimum_required(VERSION 3.16.0)

project(ImOsmWidget)

set(CXX_CMAKE_STANDARD 17)
set(CXX_CMAKE_STANDARD_REQUIRED ON)

set(IMOSMWIDGET_SOURCES
    imosmwidget/ImOsmWidget.cpp
    imosmwidget/OsmTileLoader.cpp
    imosmwidget/OsmTileTexture.cpp
)

add_library(ImOsmWidget ${IMOSMWIDGET_SOURCES})
target_include_directories(ImOsmWidget PUBLIC imosmwidget)
target_link_libraries(ImOsmWidget PUBLIC imgui)
target_link_libraries(ImOsmWidget PUBLIC implot)
target_link_libraries(ImOsmWidget PUBLIC stb)
target_link_libraries(ImOsmWidget PUBLIC curl)
target_compile_features(ImOsmWidget PUBLIC cxx_std_17)
~~~
