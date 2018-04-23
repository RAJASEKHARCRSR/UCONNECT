/*****************************************************************************
**
** uWlanHlpr.cpp
**	helper functions for the uwlan utility. many of them 
**  are OS specfic needs
**
** Author: 
** Date  : 
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

******************************************************************************/

#include "uWlanUtils.h"

#ifdef WIN32

char* ssidToString(PDOT11_SSID pSsid)
{
	char* string = (char*) malloc(pSsid->uSSIDLength+1);
	unsigned int i;
	
	for(i=0; i < pSsid->uSSIDLength; i++) 
		string[i] = pSsid->ucSSID[i];
	string[i] = '\0';
	return string;
}

char* GetBssTypeString (int dot11BssType)
{
	char* strRetCode;

	switch(dot11BssType)
	{
		case 1: strRetCode = "Infrastructure"; break;
		case 2: strRetCode = "Ad hoc"; break;
		case 3: strRetCode = "Any"; break;
		default: strRetCode = "Unknown BSS type";
	}
	return strRetCode;
}

char* GetAuthAlgoString (int dot11AuthAlgo)
{
	char* strRetCode = "Unknown algorithm";

	switch(dot11AuthAlgo)
	{
		case 1: strRetCode = "open"; break;
		case 2: strRetCode = "shared"; break;
		case 3: strRetCode = "WPA-Enterprise"; break;
		case 4: strRetCode = "WPAPSK"; break;
		case 5: strRetCode = "WPANONE"; break;
		case 6: strRetCode = "WPA2-Enterprise"; break;
		case 7: strRetCode = "WPA2PSK"; break;
		default: if ((dot11AuthAlgo & DOT11_AUTH_ALGO_IHV_START) == 0x0) 
					 strRetCode = "Vendor-specific algorithm";
	}
	return strRetCode;
}

char* GetCipherAlgoString (int dot11CipherAlgo)
{
	char* strRetCode = "Unknown algorithm";

	switch(dot11CipherAlgo)
	{
		case 0: strRetCode = "none"; break;
		case 1: strRetCode = "WEP40"; break;
		case 2: strRetCode = "TKIP"; break;
		case 4: strRetCode = "AES"; break;
		case 5: strRetCode = "WEP104"; break;
		case 256: strRetCode = "USE-GROUP"; break;
		case 257: strRetCode = "WEP"; break;
		default: if ((dot11CipherAlgo & DOT11_CIPHER_ALGO_IHV_START) == 0x0) 
					 strRetCode = "Vendor-specific algorithm";
	}
	return strRetCode;
}

int readFile (char** pData, char *file)
{
	#define DATA 1024
	FILE *pTmpFil;
	int i=0, readed;
	int finish = FALSE;

	if((pTmpFil = fopen(file, "r")) == NULL)
	{
		return -1;
	}

	*pData=NULL;
	
	while (!finish)
	{
		*pData = (char*) realloc (*pData, DATA * (i+1) +1);
		readed = fread(*pData + DATA*i, 1, DATA, pTmpFil);
		if (readed<DATA) finish=TRUE;
		i++;
	}
	(*pData)[DATA*(i-1)+readed]=0;
	fclose(pTmpFil);
	return 0;
}

void write2File (FILE *pFile, char *str)
{
	fwrite(str, strlen(str), 1, pFile);
}

void writeTag (FILE *pFile, char *str, char *startOrEnd, 
			   int level)
{
	char *pTmpStr;

	pTmpStr = (char*)malloc(strlen(str) + 7);
	if(!pTmpStr) 
	{
		return;
	}
	while(level--) 
		fwrite("    ", 4, 1, pFile);
	if(!strcmp(startOrEnd, "start"))
		sprintf(pTmpStr, "<%s>\n", str);
	else
		sprintf(pTmpStr, "</%s>\n", str);
	fwrite(pTmpStr, strlen(pTmpStr), 1, pFile);
	free(pTmpStr);
}

void writeXmlLn (FILE *pFile, char *tag, 
				 char *str, int level)
{
	char *pTmpStr;

	pTmpStr = (char*)malloc(2*strlen(tag) + strlen(str) + 7);
	if(!pTmpStr) 
	{
		return;
	}
	while(level--) 
		fwrite("    ", 4, 1, pFile);
	sprintf(pTmpStr, "<%s>%s</%s>\n", tag, str, tag);
	fwrite(pTmpStr, strlen(pTmpStr), 1, pFile);
	free(pTmpStr);
}



#endif /*WIN32*/