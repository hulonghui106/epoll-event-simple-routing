#ifndef _DEBUG_H
#define _DEBUG_H



#ifdef DEBUG

#include<stdio.h>

#define INFO(format) fprintf(stderr, format )
#define LOG(format, ...) fprintf(stderr, format , __VA_ARGS__)

#else

#define LOG(...)
#define INFO(...)

#endif // debug

#endif // debug.h
