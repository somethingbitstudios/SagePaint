//Selfmade

#pragma once
//app control, also need to change the CMAKELIST at SagePaint/SagePaint/CmakeLists.txt, switch the comment and copy the "gg.cpp" ... "gg.h"...
//#define HIDE_CONSOLE


//'toggle' to turn on/off all debug
#define DEBUG
//'toggle' to turn on (when defined) less important debug
#define DEBUG_ALL

//DLOG(x) is used for more granular logs
//IDLOG(x) is used for important events
#include <iostream>
//app control macros
#ifdef DEBUG
#ifdef DEBUG_ALL
#define DLOG(x) std::cout << x << std::endl;
#else
#define DLOG(x)
#endif
#define IDLOG(x) std::cerr << x << std::endl;
#else
#define DLOG(x)
#define IDLOG(x)

#endif

//app notes:

/*
the polymorphism approach so far:
with Model:
a) have virtual funcs and override them
+  ~performance loss
-  means Model needs to contain all functions of every model

b) fix through casting - static_cast<TargetClass*>(SourceClass*)->Thing()
+ no clutter in Model and other parent classes ()
- slight performance loss
*/

/*

*/