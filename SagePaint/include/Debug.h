#pragma once
//app control
//#define HIDE_CONSOLE
#define DEBUG




#include <iostream>
//app control macros
#ifdef DEBUG
#define DLOG(x) std::cout << x << std::endl
#else
#define DLOG(x)
#endif

