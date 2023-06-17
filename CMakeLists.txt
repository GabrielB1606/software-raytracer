cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

# Paths to dependencies
set(SDL2_PATH "${CMAKE_SOURCE_DIR}/vendor/SDL2-2.26.5")
set(IMGUI_PATH "${CMAKE_SOURCE_DIR}/vendor/imgui")
set(GLM_PATH "${CMAKE_SOURCE_DIR}/vendor/glm")

# SDL2
# Instead of building SDL2 from source, we'll link against the precompiled binaries
# SDL2
# Instead of building SDL2 from source, we'll link against the precompiled binaries
add_library(SDL2 STATIC IMPORTED)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  # 64-bit
  set_target_properties(SDL2 PROPERTIES
    IMPORTED_LOCATION "${SDL2_PATH}/x86_64-w64-mingw32/lib/libSDL2.a"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_PATH}/x86_64-w64-mingw32/include"
  )
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  # 32-bit
  set_target_properties(SDL2 PROPERTIES
    IMPORTED_LOCATION "${SDL2_PATH}/i686-w64-mingw32/lib/libSDL2.a"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_PATH}/i686-w64-mingw32/include"
  )
endif()

# ImGui
# file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
set(IMGUI_SOURCES
  ${IMGUI_PATH}/imgui.cpp
  ${IMGUI_PATH}/imgui_demo.cpp
  ${IMGUI_PATH}/imgui_draw.cpp
  ${IMGUI_PATH}/imgui_tables.cpp
  ${IMGUI_PATH}/imgui_widgets.cpp
  # Add any other ImGui source files you need here
)
add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_PATH})

# GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${GLM_PATH})

# Your executable
add_executable(${PROJECT_NAME} main.cpp)

# Linking
target_link_libraries(${PROJECT_NAME} SDL2 imgui glm)
