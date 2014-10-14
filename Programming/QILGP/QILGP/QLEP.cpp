// QLEP.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

__declspec( dllexport ) int GetInt(int n)
{
	return 0;
}



