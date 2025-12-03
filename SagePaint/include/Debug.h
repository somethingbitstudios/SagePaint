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
the current image class can change, which is nice, but it means
the data (like width etc. can't be relied on to compare old with new)

->

make it into a container class, than is immutable in most cases
just let FileManager::LoadImage(path) return ImagePtr type already in sharedptr form
or deal with complexities... no, just handle loading from outside for now
*/
