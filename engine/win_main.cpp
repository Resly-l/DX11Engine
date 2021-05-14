#include "engine_application.h"

int _stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	EngineApplication{}.Run();

	return 0;
}