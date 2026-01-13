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
#define IDLOG(x) std::cout << x << std::endl;
#else
#define DLOG(x)
#define IDLOG(x)

#endif

//app notes:

/*
* TODO:
add a UI setting that only allows integer scaling (so additive: 1,2,3... on macro, multiplicative: 2,3,4...) when magnifying


look into implementing shared_from_this() in objects, would be easier to get a pointer

shelved TODOs:
* fix non-integer scaling making inconsistent pixels (filtering toggle?), like this:
* 
* 
* if (isIntegerScale) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
} else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
in the draw call or call a special function in the canvasmanager (preferred)

NOTE: this isn't a good solution, a real solution would be very hard, might try later
* 
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
tried swap chain fencing, did not work
OK, so I've just checked, and MSPaint, GIMP AND Krita all have this same delay, so it might be unavoidable, but I'll still try to research
...
not sure if Vulkan would be better, so staying with OpenGL for now
researched Vulkan, and it seems very complex, and only worth it if used completely correctly

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

c) combination
using virtual functions for important features a lot of children might have, but for one off funcs use the cast
or simply have the ChildTypePtr because you'll need it (use getShared())
*/

/*

*/