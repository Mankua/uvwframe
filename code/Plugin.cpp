/*===========================================================================*\
 | 
 |  FILE:	Plugin.cpp
 |			Skeleton project and code for a Helper object
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Harry Denholm
 |			Developer Consulting Group
 |			Copyright(c) Discreet 1999
 |
 |  HIST:	Started 16-3-99
 | 
\*===========================================================================*/

#include "UVWFrame.h"
#include "frame_mod.h"

HINSTANCE hInstance;
int controlsInit = FALSE;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {

#ifndef MAX_RELEASE_R9

	hInstance = hinstDLL;

	if ( !controlsInit ) {
		controlsInit = TRUE;
		InitCustomControls(hInstance);
		InitCommonControls();
	}

	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		}

#else

	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		hInstance = hinstDLL;
		DisableThreadLibraryCalls(hInstance);
	}

#endif

return (TRUE);

}

__declspec( dllexport ) const TCHAR * LibDescription() 
{ 
	return GetString(IDS_LIBDESC); 
}


__declspec( dllexport ) int LibNumberClasses() 
{
	return 2;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i) 
	{
		case 0: return GetUVWFrameDesc();
		case 1: return GetUVWFrameModDesc();
		default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion() 
{
	return VERSION_3DSMAX; 
}

TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if(hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}
