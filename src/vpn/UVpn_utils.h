#ifndef _UVPN_UTILS_H_
#define _UVPN_UTILS_H_


wchar_t* stringToWString (const char* src);
int unsignedShortStringLength (const wchar_t* src);
void wCharToUCharString (const wchar_t* src, unsigned char* dest);


#endif /*_UVPN_UTILS_H_*/