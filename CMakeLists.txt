cmake_minimum_required(VERSION 3.16.0)

project(imosm)

set(CXX_CMAKE_STANDARD 20)
set(CXX_CMAKE_STANDARD_REQUIRED ON)

set(IMOSM_SRC
    ImOsmTile.cpp
    ImOsmTileLoader.cpp
    ImOsmTileSourceAsync.cpp
    ImOsmTileGrabberUrl.cpp
    ImOsmTileGrabber.cpp
    ImOsmMapPlot.cpp
    ImOsmTileLoaderUrl.cpp
    ImOsmTileTexture.cpp
)

add_library(${PROJECT_NAME} ${IMOSM_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC implot)
target_link_libraries(${PROJECT_NAME} PUBLIC stb)
target_link_libraries(${PROJECT_NAME} PUBLIC curl)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
