cmake_minimum_required(VERSION 3.13)

project(MyOpenGl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

if (MSVC)
    add_compile_options(/W4)
    set_source_files_properties(${GLAD_SOURCE} PROPERTIES COMPILE_FLAGS "/w")
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-missing-designated-field-initializers)
    set_source_files_properties(${GLAD_SOURCE} PROPERTIES COMPILE_FLAGS "-w")
endif()

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
