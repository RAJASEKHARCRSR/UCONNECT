#include "UCommonFunctions.h"

/***********************************************************************
*	signedToFloat -
*	Converts a 1.15.16 value into a double
*
*	Parameters
*	u32Value			- Value to be converted
*   u32SignBit			- Sign bit of the value
*	u32IntegerBits		- Integer bits
*	u32FractionBits		- Fraction bits
***********************************************************************/
double signedToFloat(unsigned int u32Value, 
					 unsigned int u32SignBit, 
					 unsigned int u32IntegerBits, 
					 unsigned int u32FractionBits)
{
	unsigned int u32IFBits    = u32IntegerBits + u32FractionBits;
	unsigned int u32TotalBits = (u32SignBit ? 1 : 0) + u32IFBits;

	unsigned int u32SignBitMask = (1 << (u32IFBits));
	unsigned int u32Mask = u32TotalBits == 32 ? 0xFFFFFFFF : (1 << u32TotalBits) - 1;

	double dValue, dScale;

	// Mask out only the required bits
	u32Value &= u32Mask;

	// Signed & unsigned conversions differ
	if (u32SignBit)
				  {
				  // Sign extend to 32 bits
				  if (u32Value & u32SignBitMask)
								 u32Value |= ~u32Mask;
				  dValue = (double) ((signed int) u32Value);
				  }
	else
				  dValue = (double) u32Value;

	dScale = (double) (1 << u32FractionBits);

	return dValue/dScale;
}



void tokeniseInformation(std::string &rspStr, std::vector<std::string>& vStr, char ch)
{
	std::string s = rspStr;
	vStr.clear();
	std::string::size_type prev_pos = 0, pos = 0;     
	while( (pos = s.find(ch, pos)) != std::string::npos )     
	{         
		std::string substring( s.substr(prev_pos, pos-prev_pos) );
		vStr.push_back(substring);      
		prev_pos = ++pos;     
	}     
	std::string substring( s.substr(prev_pos, pos-prev_pos) ); 
	vStr.push_back(substring);
	return;
}

void replaceAll(string& s , string strToReplace , string stringToReplaceWith) 
{
    int j = 0;
	unsigned int replaceStrLength =  strToReplace.length();
	unsigned int replaceWithStrLength =  stringToReplaceWith.length();
	for (unsigned int i = 0; i < s.length(); i++) {
	    
		j = s.find(strToReplace);
		if (j != string::npos) {  // stringToReplace exists in the given string.
            
            s.replace(j , replaceStrLength , stringToReplaceWith);
            i = j + replaceWithStrLength;
		} else {  // stringToReplace doesn't exit -- break loop.
            break;
		}
	}
    
	return ;
}
