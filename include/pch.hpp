#pragma once

// #include <glad/gl.h>

// #define GLEW_STATIC
// #include <GL/glew.h>
#include <glad/glad.h>

#ifdef _WIN32

#undef APIENTRY

#ifndef _SILENCE_CXX23_DENORM_DEPRECATION_WARNING
#define _SILENCE_CXX23_DENORM_DEPRECATION_WARNING
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

// #ifdef GLEW_EGL
	// static_assert(false && "EGL");
// #else
	// static_assert(false && "Not EGL");
// #endif

#include <cstddef> 
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <numeric>
#include <cassert>
#include <algorithm>
#include <array>
#include <sstream>

#include <wx/glcanvas.h>

#include <wx/image.h>
