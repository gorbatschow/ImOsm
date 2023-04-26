cmake_minimum_required(VERSION 3.16.0)

project(imosm)

set(CXX_CMAKE_STANDARD 20)
set(CXX_CMAKE_STANDARD_REQUIRED ON)

set(IMOSM_SRC
    src/ImOsmTile.cpp
    src/ImOsmTileLoader.cpp
    src/ImOsmTileSourceAsync.cpp
    src/ImOsmTileSourceUrl.cpp
    src/ImOsmTileGrabberUrl.cpp
    src/ImOsmTileGrabber.cpp
    src/ImOsmMapPlot.cpp
    src/ImOsmTileLoaderUrl.cpp
    src/ImOsmTileTexture.cpp
    src/ImOsmRichMarkItem.cpp
    src/ImOsmRichMarkStorage.cpp
    src/ImOsmRichMarkEditorWidget.cpp
    src/ImOsmRichMarkItemWidget.cpp
    src/ImOsmRichDestinationCalcWidget.cpp
    src/ImOsmRichDistanceCalcWidget.cpp
    src/ImOsmRichMapPlot.cpp
    src/ImOsmTileGrabberWidget.cpp
    src/ImOsmTileSourceWidget.cpp
    src/ImOsmTileSourceUrlImpl.cpp
    src/ImOsmTileSourceFs.cpp
    src/ImOsmTileSaver.cpp
)

add_library(${PROJECT_NAME} ${IMOSM_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_include_directories(${PROJECT_NAME} PUBLIC ./include)
target_link_libraries(${PROJECT_NAME} PUBLIC latlon)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC implot)
target_link_libraries(${PROJECT_NAME} PUBLIC stb)
target_link_libraries(${PROJECT_NAME} PUBLIC curl)
target_link_libraries(${PROJECT_NAME} PUBLIC mini)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
