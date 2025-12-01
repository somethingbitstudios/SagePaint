//Selfmade

#pragma once
//app control, also need to change the CMAKELIST at SagePaint/SagePaint/CmakeLists.txt, switch the comment and copy the "gg.cpp" ... "gg.h"...
//#define HIDE_CONSOLE
#define DEBUG




#include <iostream>
//app control macros
#ifdef DEBUG
#define DLOG(x) std::cout << x << std::endl;
#else
#define DLOG(x)
#endif

