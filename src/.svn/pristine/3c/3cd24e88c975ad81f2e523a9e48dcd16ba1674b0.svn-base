/*****************************************************************************
**
** uwlanUtils.h
**	header file to use wlan helper functions.
**
** Author: 
** Date  : 
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

******************************************************************************/

#ifndef UWLAN_HLPR_H
#define UWLAN_HLPR_H

#ifdef WIN32
char* ssidToString(PDOT11_SSID pSsid);
char* GetBssTypeString (int dot11BssType);
char* GetAuthAlgoString (int dot11AuthAlgo);
char* GetCipherAlgoString (int dot11CipherAlgo);
wchar_t* stringToWString (const char* src);
int readFile (char** pData, char *file);
void write2File (FILE *pFile, char *str);
void writeTag (FILE *pFile, char *str, char *startOrEnd, 
			   int level);
void writeXmlLn (FILE *pFile, char *tag, 
				 char *str, int level);
int readFile (char** pData, char *file);

#endif /*WIN32*/

#endif /*UWLAN_HLPR_H*/