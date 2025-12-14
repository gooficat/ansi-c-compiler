#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef FANCC_IMPLEMENTATION
HANDLE STDOUTPUTHANDLE;
#else
extern HANDLE STDOUTPUTHANDLE;
#endif

#define BLUE FOREGROUND_BLUE
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define WHITE RED | GREEN | BLUE
#define YELLOW RED | GREEN
#define PURPLE RED | BLUE

#define SETCOLOR(color) SetConsoleTextAttribute(STDOUTPUTHANDLE, color)

#define init_fancc STDOUTPUTHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
#else
#define WHITE
#define SETCOLOR(color)
#define init_fancc
#endif

#ifdef __cplusplus
#include <iostream>
#define FANCYPRINT(...) std::cout << __VA_ARGS__
#else
#include <stdio.h>
#define FANCYPRINT(...) printf_s(__VA_ARGS__)
#endif

#define fancc(c, ...)                                                                              \
   SETCOLOR(c);                                                                                    \
   FANCYPRINT(__VA_ARGS__);
