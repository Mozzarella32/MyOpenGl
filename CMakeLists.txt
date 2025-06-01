cmake_minimum_required(VERSION 3.13)

project(MyOpenGl LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# WxWidgets
find_package(wxWidgets QUIET)

# if (NOT wxWidgets_FOUND) message(STATUS "wxWidgets not found")
set(wxBUILD_SHARED OFF)
set(wxBUILD_TESTS OFF)
set(wxBUILD_DEMOS OFF)
set(wxBUILD_EXAMPLES OFF)

set(wxBUILD_STC
    OFF
    CACHE BOOL "" FORCE)
set(wxUSE_STC
    OFF
    CACHE BOOL "" FORCE)

message(STATUS "Fetching wxWidgets...")
FetchContent_Declare(
  wxWidgets
  GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
  GIT_SHALLOW ON
  CMAKE_ARGS
  -DwxBUILD_SHARED=OFF
  -DwxBUILD_TESTS=OFF
  -DwxBUILD_DEMOS=OFF
  -DwxBUILD_EXAMPLES=OFF
  -DwxUSE_WEBVIEW=OFF
  -DwxUSE_WEBREQUEST=OFF
  -DwxUSE_MEDIACTRL=OFF
  -DwxUSE_SOCKETS=OFF
  -DwxUSE_PROTOCOL=OFF
  -DwxUSE_URL=OFF
  -DwxUSE_WEBVIEW_EDGE=OFF
  -DwxUSE_GRAPHICS_CONTEXT=OFF
  -DwxUSE_ODBC=OFF
  -DwxUSE_ZIPSTREAM=OFF
  -DwxUSE_TARSTREAM=OFF
  -DwxUSE_FTP=OFF
  -DwxUSE_HTTP=OFF
  -DwxUSE_STL=OFF
  -DwxBUILD_STC=OFF)
FetchContent_MakeAvailable(wxWidgets)
# else() message(STATUS "wxWidgets Found") endif()

# GLEW find_package(GLEW QUIET)

# if (NOT GLEW_FOUND) message(STATUS "GLEW not found") message(STATUS "Fetching
# GLEW...")

set(glew-cmake_BUILD_SHARED OFF)
set(USE_GLU ON)
if(UNIX AND NOT APPLE) # Nur Linux, nicht macOS
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGLEW_EGL")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_EGL")
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/glew-cmake)

# set(GLEW_X11 OFF CACHE BOOL "" FORCE) set(GLEW_EGL ON CACHE BOOL "" FORCE)
# FetchContent_Declare( GLEW GIT_REPOSITORY
# https://github.com/Perlmint/glew-cmake.git GIT_SHALLOW ON )

# FetchContent_MakeAvailable(glew)

# GLEW with modern CMake and options FetchContent_Declare( glew GIT_REPOSITORY
# https://github.com/Perlmint/glew-cmake.git GIT_TAG        master )

# # Tell GLEW to build statically, with EGL and without X11
# set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) set(GLEW_USE_STATIC_LIBS ON
# CACHE BOOL "" FORCE) set(GLEW_EGL ON CACHE BOOL "" FORCE) set(GLEW_NO_X11 ON
# CACHE BOOL "" FORCE)

# FetchContent_MakeAvailable(glew) #add_subdirectory(${glew_SOURCE_DIR}
# ${glew_BINARY_DIR} EXCLUDE_FROM_ALL) else() message(STATUS "GLEW Found")
# endif()

# STB - Header-only-Bibliothek message(STATUS "Fetching stb...")

# FetchContent_Declare( stb GIT_REPOSITORY https://github.com/nothings/stb.git
# GIT_SHALLOW ON )

# FetchContent_MakeAvailable(stb)

set(STB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/submodules/stb")

# GLAD set(GLAD_SOURCE_DIR "${PROJECT_SOURCE_DIR}/thirdparty/glad/src")
# set(GLAD_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/thirdparty/glad/include")

# file(GLOB_RECURSE GLAD_SOURCE CONFIGURE_DEPENDS "${GLAD_SOURCE_DIR}/*")

# Directories
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")

set(PCH_HEADERS "${INCLUDE_DIR}/pch.hpp")

file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${SRC_DIR}/*")

file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${INCLUDE_DIR}/*")

source_group("Source Files" FILES ${MY_SOURCE})
source_group("Header Files" FILES ${MY_HEADERS})

list(REMOVE_ITEM MY_SOURCE ${PCH_HEADERS})
list(REMOVE_ITEM MY_HEADERS ${PCH_HEADERS})

# Statische Bibliothek erstellen
add_library(${PROJECT_NAME} STATIC ${MY_SOURCE} ${GLAD_SOURCE})

# Precompiled Header (falls verwendet)
target_precompile_headers(${PROJECT_NAME} PRIVATE
                          $<$<COMPILE_LANGUAGE:CXX>:${PCH_HEADERS}>)

# Include-Direktiven
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR}
                                                  ${STB_INCLUDE_DIR})

# wxWidgets Linking
target_link_libraries(${PROJECT_NAME} PUBLIC wx::core wx::base)

# if (NOT GLEW_FOUND)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)
# else() target_link_libraries(${PROJECT_NAME} PUBLIC GLEW::glew_s) endif()
