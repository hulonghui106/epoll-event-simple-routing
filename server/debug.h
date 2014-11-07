#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef DEBUG

#include<stdio.h>

#define LOG printf

#else

#define LOG /##/


#endif // debug

#endif // debug.h
