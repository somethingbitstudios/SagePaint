#include "SagePaint.h"
#include "app/MainApp.h"

#ifdef HIDE_CONSOLE
int WinMain()
{
	MainApp();
	return 0;
}
#else
int main()
{
	MainApp();
	return 0;
}
#endif