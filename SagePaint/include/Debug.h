//Selfmade

#pragma once
//app control, also need to change the CMAKELIST at SagePaint/SagePaint/CmakeLists.txt, switch the comment and copy the "gg.cpp" ... "gg.h"...
//#define HIDE_CONSOLE


//'toggle' to turn on/off all debug
#define DEBUG
//'toggle' to turn on (when defined) less important debug
#define DEBUG_ALL

#define WINDOWS_BUILD

//#define LINUX_BUILD

#ifdef WINDOWS
#define WIN_ONLY(x) x
#else
#define WIN_ONLY(x)
#endif
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
* 
* writing part:
* there is currently some delay between the mouse and program
* https://wikis.khronos.org/opengl/Sync_Object#Synchronization
* might fix it? it seems to be a timing issue
* NO
* maybe using vulkan could be worth it...
* 
* trying to get mouse from coordinates directly
* POINT p;
		GetCursorPos(&p);                    // screen coords
		ScreenToClient(glfwGetWin32Window(window), &p);  // window-local coords

		int xpos = p.x;
		int ypos = p.y;
still lagging behind a bit, maybe the mouse is not the issue
* 
* 
* 
* 
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