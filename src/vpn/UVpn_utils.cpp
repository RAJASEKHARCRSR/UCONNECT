/* Utils for VPN implementation
 * Merge with WiFi utils
 */

/* suppress c_safe warnings for the time being */
#define _CRT_SECURE_NO_WARNINGS
#ifndef UNICODE
#define UNICODE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



wchar_t* stringToWString (const char* src)
{
	int i;
	int length = strlen(src);
	wchar_t* unsignedShortString = (wchar_t*) malloc((length+1) * sizeof(wchar_t));
	
	for (i = 0; i < length; i++) 
		unsignedShortString[i] = src[i];
	
	unsignedShortString[i] = '\0';
	
	return unsignedShortString;	
}

int unsignedShortStringLength (const wchar_t* src)
{
	if (src==NULL) return 0;
	
	int i = 0;
	while (src[i] != 0) i++;
	return i;
}

void wCharToUCharString (const wchar_t* src, unsigned char* dest)
{
	int i;
	int length;

	if(src == NULL)
	{ 
		dest = NULL; 
		return; 
	}
	
	length = unsignedShortStringLength(src);

	for (i = 0; i < length; i++) 
		dest[i] = src[i];
	dest[i] = '\0';
}
