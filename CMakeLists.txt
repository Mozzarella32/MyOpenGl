cmake_minimum_required(VERSION 3.13)

project(MyOpenGl LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# WxWidgets
set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS OFF CACHE BOOL "" FORCE)
set(wxBUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(wxBUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(wxUSE_WEBVIEW OFF CACHE BOOL "" FORCE)
set(wxUSE_WEBREQUEST OFF CACHE BOOL "" FORCE)
set(wxUSE_MEDIACTRL OFF CACHE BOOL "" FORCE)
set(wxUSE_SOCKETS OFF CACHE BOOL "" FORCE)
set(wxUSE_PROTOCOL OFF CACHE BOOL "" FORCE)
set(wxUSE_FS_INET OFF CACHE BOOL "" FORCE)
set(wxUSE_URL OFF CACHE BOOL "" FORCE)
set(wxUSE_WEBVIEW_EDGE OFF CACHE BOOL "" FORCE)
set(wxUSE_ODBC OFF CACHE BOOL "" FORCE)
set(wxUSE_ZIPSTREAM OFF CACHE BOOL "" FORCE)
set(wxUSE_TARSTREAM OFF CACHE BOOL "" FORCE)
set(wxUSE_FTP OFF CACHE BOOL "" FORCE)
set(wxUSE_HTTP OFF CACHE BOOL "" FORCE)
set(wxUSE_STL OFF CACHE BOOL "" FORCE)
set(wxBUILD_STC OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/wxWidgets)
set(GLAD_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/thirdparty/glad/gl/include")
set(GLAD_SOURCE "${PROJECT_SOURCE_DIR}/thirdparty/glad/gl/src/glad.c")


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

set(STB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/submodules/stb")

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
                                                  ${STB_INCLUDE_DIR}
                                                  ${GLAD_INCLUDE_DIR})

# wxWidgets Linking
target_link_libraries(${PROJECT_NAME} PUBLIC wx::core wx::base)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
)
