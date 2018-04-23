#ifndef UCOMMON_FUNCTIONS_H
#define UCOMMON_FUNCTIONS_H

#include <vector>
#include <string>

using namespace std;

double signedToFloat(unsigned int u32Value, 
					 unsigned int u32SignBit, 
					 unsigned int u32IntegerBits, 
					 unsigned int u32FractionBits);

void tokeniseInformation(std::string &rspStr, std::vector<std::string>& vStr, char ch);
void replaceAll(string& s , string strToReplace , string stringToReplaceWith);



#endif //UCOMMON_FUNCTIONS_H