#ifndef _UCOMMON_UTILS_H_
#define _UCOMMON_UTILS_H_

#include "UTypes.h"
wchar_t* stringToWString (const char* src);
int unsignedShortStringLength (const wchar_t* src);
void wCharToUCharString (const wchar_t* src, unsigned char* dest);

unsigned char *ussd_encode (unsigned char *toEncode_p, unsigned int *retLen_p, 
							unsigned int len);
unsigned char *ussd_decode (char *toDecodeAscii_p, unsigned int *retLen_p, 
							unsigned int len);
U_SINT8 *stristr(U_SINT8* szStringToBeSearched,
				const U_SINT8* szSubstringToSearchFor);
#endif