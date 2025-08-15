#pragma once
#include "pch.hpp"

#ifndef NDEBUG

extern void _GLGetError(const char* file, int line, const char* call);

extern void _ErrorLog(const char* file, int line, const char* message);

extern void _GLContextCheck();

#define GLCALL(call) call; _GLGetError(__FILE__, __LINE__, #call)
#define ERRORLOG(string)  _ErrorLog(__FILE__,__LINE__,#string)
#define CONTEXTCHECK _GLContextCheck()

#else

#define GLCALL(call) call
#define ERRORLOG(call) do {} while (0)
#define CONTEXTCHECK(call) do {} while (0)

#endif
