#include "UCommonUtils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
		dest[i] = (unsigned char)src[i];
	dest[i] = '\0';
}

/* Convert a hex number to ascii representation */
static char changeToAscii (unsigned char cc)
{
	char rr;

	if((cc >= 0) && (cc <= 9))
		rr = '0' + cc;	
	else if((cc >= 0xa) && (cc <= 0xf))
		rr = 'a' + (cc-0xa);
	return rr;
}

#define CONVERT_ENCODED_STR_TO_ASCII(encoded_p, asciiStr_p, reqLen) \
do {\
	int mlen = 0;\
	int mii = 0;\
	while(mii <= reqLen)\
	{\
		asciiStr_p[mlen++] = changeToAscii(encoded_p[mii] >> 4);\
		asciiStr_p[mlen++] = changeToAscii(encoded_p[mii] & 0xf);\
		mii++;\
	}\
}while(0)

/* This function converts an ascii string to a padded USSD format.
* PARAMETERS
* toEncode_p -	Pointer to the ascii array
* retLen_p	 -  Pointer to the return array length
* len		 -  len of the array
*
* RETURN
* Pointer to the converted USSD array of padded bytes on SUCCESS
* NULL on Failure
* Memory to be freed by the caller
*/
unsigned char *ussd_encode (unsigned char *toEncode_p, unsigned int *retLen_p, 
							unsigned int len)
{
	unsigned char *encoded_p = NULL, *asciiStr_p = NULL;
	int reqLen = 0, asciiLen = 0, ii = 0;
	int idxFrm = 0, idxTo = 0;

	*retLen_p = 0;
	if(!len || !toEncode_p || !retLen_p) 
		goto err1;

	/* allocate mem for encoded hex string */
	reqLen = ((len * 7) % 8)? ((len * 7) / 8) + 1: ((len * 7) / 8);
	if(!(encoded_p = (unsigned char*)malloc(reqLen)))
		goto err1;

	memset(encoded_p, 0, reqLen);

	/* encode the string by packing into 8 bit boundaries */
	encoded_p[idxTo] = toEncode_p[idxFrm++];
	while(--len)
	{
		if(idxFrm % 8 == 0)
		{
			encoded_p[idxTo] = toEncode_p[idxFrm];
			/* skip one */
		}
		else
		{
			encoded_p[idxTo] |= (toEncode_p[idxFrm] << (8-(idxFrm % 8)));
			encoded_p[++idxTo] = toEncode_p[idxFrm] >> idxFrm % 8;
		}
		idxFrm++;
	}
	if(idxTo % 8 == 7) /* spec requires us to append a CR */
	{
		if(!(encoded_p[idxTo-1] & 0xfe))
			encoded_p[idxTo-1] |= 0x0d << 1;
	}

	/* we need to return Ascii !! */
	asciiLen = (reqLen * 2) + 1; /* Each encoded nibble is again ascii! */
	if(!(asciiStr_p = (unsigned char*)malloc(asciiLen)))
		goto err2;
	memset(asciiStr_p, 0, asciiLen);
	CONVERT_ENCODED_STR_TO_ASCII(encoded_p, asciiStr_p, reqLen);
	asciiStr_p[asciiLen - 1] = '\0';
	*retLen_p = asciiLen - 1; /* as we add a \0 extra */

err2:
	free(encoded_p);
err1:
	return asciiStr_p;
}

/* converts an ascii hex number into equivalent hex */
unsigned char changeToXX (char cc)
{
	unsigned char kk = 0;

	if((cc >= '0') && (cc <= '9'))
		kk = 0 + cc - '0';
	else if((cc >= 'a') && (cc <= 'f'))
		kk = 0xa + cc - 'a';
	else if((cc >= 'A') && (cc <= 'F'))
		kk = 0xa + cc - 'A';

	return kk;
}


#define CHANGE_ENCODED_STR_TO_HEX(ascii_p, nonAscii_p, nonAsciiLen)\
do {\
	int mii = 0, mkk = 0, mLen = nonAsciiLen;\
	int totLen = (nonAsciiLen * 2) + 1;\
	while(mLen)\
	{\
		nonAscii_p[mii] = (changeToXX(ascii_p[mkk++]) << 4);\
		if(mkk < totLen)\
			nonAscii_p[mii] |= (changeToXX(ascii_p[mkk++]) & 0xf);\
		mii++;\
		mLen--;\
	}\
}while(0)

/* Converts a padded USSD format char array into its ASCII form
* PARAMETERS
* toDecode_p - Pointer to the padded array
* retLen_p	 -  Pointer to the return array length
* len - len of the padded array
*
* RETURNS
* Pointer to the ASCII array
* NULL on error
* Memory to be freed by the caller
*/
unsigned char *ussd_decode (char *toDecodeAscii_p, unsigned int *retLen_p, 
							unsigned int len)
{
	unsigned char *decoded_p = NULL;
	unsigned char *toDecode_p = NULL;
	int idxFrm = 0, idxTo = 0;
	int reqLen = 0;
	int ii = 0, decodeLen = 0;

	if(!toDecodeAscii_p || !len || !retLen_p) return NULL;

	/* An ascii string is passed here. So, convert to digits */
	decodeLen = (len % 2)? (len / 2) + 1: (len / 2);
	if(!(toDecode_p = (unsigned char*)malloc(decodeLen)))
		goto err1;
	memset(toDecode_p, 0, decodeLen);
	CHANGE_ENCODED_STR_TO_HEX(toDecodeAscii_p, toDecode_p, decodeLen);

	/* allocate mem for decoded str */
	reqLen = (decodeLen * 8) % 7? (decodeLen * 8) / 7 + 1: (decodeLen * 8) / 7;
	if(!(decoded_p = (unsigned char*)malloc(reqLen)))
		goto err2;
	memset(decoded_p, 0, reqLen);


	*retLen_p = reqLen;
	while(ii++ < reqLen)
	{
		if(idxTo % 8 == 0)
		{
			/*skip*/
			decoded_p[idxTo] = toDecode_p[idxFrm] & 0x7F;
		}
		else
		{
			idxFrm++;
			decoded_p[idxTo] = 0x7F & ((toDecode_p[idxFrm-1] >> (8-(idxTo % 8))) |
			(toDecode_p[idxFrm] << ((idxTo % 8) == 7? 8: (idxTo % 8))));
		}
		idxTo++;
	}

err2:
	free(toDecode_p);
err1:
	return decoded_p;
}

/* Checks for a substring in another string, case-insensitive
* PARAMETERS
* szStringToBeSearched		- string to be searched in
* szSubstringToSearchFor	- substring to search
*
* RETURNS
* Pointer to the ASCII array
* NULL on error
* Memory to be freed by the caller
*/

U_SINT8 *stristr(U_SINT8* szStringToBeSearched,
				const U_SINT8* szSubstringToSearchFor)
{
	U_SINT8* pPos = NULL;
	U_SINT8* szCopy1 = NULL;
	U_SINT8* szCopy2 = NULL;
	 
	// verify parameters
	if ( szStringToBeSearched == NULL || szSubstringToSearchFor == NULL )
	{
		return szStringToBeSearched;
	}
	 
	// empty substring - return input (consistent with strstr)
	if ( strlen(szSubstringToSearchFor) == 0 ) 
	{
		return szStringToBeSearched;
	}
	 
	szCopy1 = _strdup(szStringToBeSearched);
	szCopy2 = _strdup(szSubstringToSearchFor);
	
	if ( szCopy1 == NULL || szCopy2 == NULL ) 
	{
		// another option is to raise an exception here
		free((void*)szCopy1);
		free((void*)szCopy2);
		return NULL;
	}
	
	/* Convert the strings to lower case */
	U_UINT32 i = 0;
	for(i = 0; i < strlen(szCopy1); i++)
	{
		szCopy1[i] = tolower(szCopy1[i]);
	}

	for(i = 0; i < strlen(szCopy2); i++)
	{
		szCopy2[i] = tolower(szCopy2[i]);
	}

	pPos = strstr(szCopy1, szCopy2);
	 
	if ( pPos != NULL ) 
	{
		// map to the original string
		pPos = szStringToBeSearched + (pPos - szCopy1);
	}
	 
	free((void*)szCopy1);
	free((void*)szCopy2);
	 
	return pPos;
} // stristr(...)