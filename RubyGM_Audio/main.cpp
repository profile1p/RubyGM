#include "stdafx.h"
#include "include.h"

#define TEST_MAIN

#ifdef TEST_MAIN
int main(){
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{

		}
		// COM
		CoUninitialize();
	}
	return EXIT_SUCCESS;
}
#endif