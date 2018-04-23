#ifndef XML_PARSER_H_
#define XML_PARSER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "UTypes.h"
#include "uconn_Event.h"


using namespace std;


class XMLParser
{
private:
	ifstream readCfgFile;
	string readString;
	string paramName;
	string paramValue;
	unsigned int pos;
	
	string strBeforeStartTagMatch;
	string strAfterEndTagMatch;
	string strAfterMatch;
public:
	string configFileContent;
	Ras_Details_s rasDetails;

	XMLParser()
	{
		particularNM = 0;
		readString.clear();
		paramName.clear();
		paramValue.clear();
		configFileContent.clear();
		strBeforeStartTagMatch.clear();
		strAfterEndTagMatch.clear();
		strAfterMatch.clear();
	}
	~XMLParser()
	{

	}

	void toLowerCase(std::string &str);
	bool readTillChar(ifstream &readCfgFile,char ch);
	void parseConfigFile();
	void writeToConfigFile();
	string convertIntToString(int number);
	void updateCMInfo();
	CM_Config_Info_s gCMInfo;
	int particularNM; //1 for LTE, 2 for 3g, 3 for wifi

	void updateStr(string &dumpStr,string startTag,string endTag,string &str);
	void updateValue(string &id,string& value,bool isValueTag=true);
};



#endif //XML_PARSER_H_