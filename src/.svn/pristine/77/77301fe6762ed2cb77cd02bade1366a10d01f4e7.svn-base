/*****************************************************************************
**
** ThreeGParser.cpp
** Implements Parsing of AT Commands Responses or Unsolicited Responses from 
** 3G networks.
** Author: Mukta Sharma
** Date  : 20-APR-2012
** (c) United Mobile Apps Pvt Ltd (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
	0.1		20/04/12     MSH		Creation
****************************************************************************/
#include "ThreeGParser.h"

UThreeGParser::UThreeGParser(UNetworkManager* p_NM):UBufferParser(p_NM)
{

}


/* Process the AT commands responses received from the Modem */
void UThreeGParser:: processAtCommandResp(string &atCommandRsp)
{
	vector<string> vStr;
	string tmpStr = "";
	setEventID(0);
	int size = 0;
	//cout << "The Response for AT command received is:" << atCommandRsp << endl;

	if(strstr(atCommandRsp.c_str(),"+COPS:")) 
	{
		/* Process the Response of COPS (PLMN Search) command recd from Modem. */
		tokeniseInformation(atCommandRsp,vStr,':');

		tmpStr.clear();
		tmpStr = vStr[1];
		vStr.clear();
		tokeniseInformation(tmpStr,vStr,',');
		size = vStr.size();
		if ( size == 4 )
		{
			char NetworkName[50];
			strncpy( NetworkName, vStr[2].c_str(), vStr[2].size());
			NetworkName[vStr[2].size()] = '\0';
			parsedInfo.setNetworkName(NetworkName);
			setEventID(UCONN_EVT_NM_NETWORK_UPDATE);
			vStr.clear();
		}
	}
	else if(strstr(atCommandRsp.c_str(),"+CSQ:"))	
	{
		tokeniseInformation (atCommandRsp,vStr, '\n' ) ;
		size = vStr.size();
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"+CSQ:"))
			{
				tmpStr.clear();
				tmpStr = vStr[i];
				vStr.clear();
				tokeniseInformation(tmpStr,vStr,':');

				/* Process the Response of AT+CSQ (Signal strength) command recd from Modem. */
				tmpStr.clear();
				tmpStr = vStr[1];
				vStr.clear();

				/*tokenise parameters here*/
				tokeniseInformation(tmpStr,vStr);

				if(vStr.size() >= 2)
				{
					parsedInfo.setRSSI( atoi(vStr[0].c_str()));
					parsedInfo.setCINR( atoi(vStr[1].c_str()));
					setEventID(UCONN_RSP_NM_RSSI_CINR_UPDATE);
					vStr.clear();
				}
				break;
			}
		}
	}
	else if( ( strstr(atCommandRsp.c_str(),"+CGREG:")) || ( strstr(atCommandRsp.c_str(),"+CREG:")))
	{
		/*Parsing of +CEREG: response here*/
		tokeniseInformation (atCommandRsp,vStr, '\n' ) ;
		size = vStr.size();
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"+CREG:") || (strstr(vStr[i].c_str(),"+CGREG:")))
			{
				if(strstr(vStr[i].c_str(),","))
				{
					tmpStr.clear();
					tmpStr = vStr[i];
					vStr.clear();
					tokeniseInformation(tmpStr,vStr,',');
					int size = vStr.size();
					if(size == 2)
					{
						tmpStr.clear();
						tmpStr = vStr[1];
						parsedInfo.setModem_State(atoi(tmpStr.c_str()));
						setEventID(UCONN_EVT_NM_MODEM_STATE_UPDATE);
						break;
					}
					else if (size == 5)
					{
						tmpStr.clear();
						tmpStr = vStr[1];
						parsedInfo.setModem_State(atoi(tmpStr.c_str()));
						tmpStr.clear();
						tmpStr = vStr[4];
						parsedInfo.setNetworkAccessType(atoi(tmpStr.c_str()));
						setEventID(UCONN_EVT_NM_MODEM_STATE_NETWORK_ACCESS_UPDATE);
						break;
					}
					break;
				}
				else
				{
					tmpStr.clear();
					tmpStr = vStr[i];
					vStr.clear();
					tokeniseInformation(tmpStr,vStr,':');
					tmpStr = vStr[1];
					parsedInfo.setModem_State(atoi(tmpStr.c_str()));
					setEventID(UCONN_EVT_NM_MODEM_STATE_UPDATE);
					break;
				}
			}
		}
	}
	else if ( strstr(atCommandRsp.c_str(),"+CAD"))
	{

	}
	else if(strstr(atCommandRsp.c_str(),"Manufacturer:"))
	{
		/* Process the Response of ATI command recd from Modem. */
		/* Set the manufacturer[20] in the ThreeG_NM_Info struct. */

		// To be displayed in the GUI??
		// manufacturer = ;
		tokeniseInformation(atCommandRsp,vStr,'\n');

		size = vStr.size();
		string ModemInfo = "";
		int FirstIndex, LastIndex;
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"Manufacturer:"))
			{
				FirstIndex = i;
			}
			if(strstr(vStr[i].c_str(),"OK"))
			{
				LastIndex = i;
				break;
			}
		}
		for ( int ii = FirstIndex; ii < LastIndex; ii ++)
		{
			ModemInfo += vStr[ii];
			ModemInfo +="\n";
		}
		parsedInfo.setVersionInfo(ModemInfo);
		setEventID(UCONN_EVT_NM_MANUFACTURE_UPDATE);
	}
	else if(strstr(atCommandRsp.c_str(),"^HWVER:"))
	{
		/* Process the Response of AT^HWVER command recd from Modem. */
	}
	else if(strstr(atCommandRsp.c_str(),"^SYSCFG:"))
	{
		/* Process the Response of AT^SYSCFG command recd from Modem. */
		/* Response of ^SYSCFG message -  
		<mode> system mode reference:
		2 Automatic search
		13 GSM ONLY
		14 WCDMA ONLY
		16 No change

		<acqorder> Network access sequence reference:
		0 Automatic search
		1 GSM first, WCDMA later
		2 WCDMA first, GSM later
		3 No change

		<band> The band of frequency relate to selection of mode, which is actually up to the
		performance of MS. The parameter is HEX string, whose value is as follows or with the
		exception of 0x3FFFFFFF and 0x40000000 parameter as combination:
		00080000 (CM_BAND_PREF_GSM_850) GSM 850
		00000080（CM_BAND_PREF_GSM_DCS_1800） GSM DCS systems
		00000100（CM_BAND_PREF_GSM_EGSM_900） Extended GSM 900
		00000200（CM_BAND_PREF_GSM_PGSM_900） Primary GSM 900
		00100000（CM_BAND_PREF_GSM_RGSM_900） Railway GSM 900
		00200000（CM_BAND_PREF_GSM_PCS_1900） GSM PCS
		00400000（CM_BAND_PREF_WCDMA_I_IMT_2000） WCDMA IMT 2000
		00800000 (CM_BAND_PREF_WCDMA_II_PCS_1900) WCDMA_II_PCS_1900
		04000000 (CM_BAND_PREF_WCDMA_V_850) WCDMA_V_850
		0002000000000000 (CM_BAND_PREF_WCDMA_VIII_900) WCDMA_VIII_900
		
		<roam> Roaming support:
		0 Not supported
		1 Roaming is supported
		2 No change
	
		<srvdomain> domain setting:
		0 CS_ONLY
		1 PS_ONLY
		2 CS_PS
		3 ANY
		4 No change */ 
	}
	else if(strstr(atCommandRsp.c_str(),"+CFUN:"))
	{
		/* Parsing of the +CFUN: response here */
		tokeniseInformation (atCommandRsp,vStr, '\n' ) ;
		size = vStr.size();
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"+CFUN:"))
			{
				tmpStr.clear();
				tmpStr = vStr[i];
				vStr.clear();
				tokeniseInformation(tmpStr,vStr,':');
				tmpStr = vStr[1];
				parsedInfo.setPowerup_state(atoi(vStr[1].c_str()));
				if(parsedInfo.getPowerup_state() == 1)
				{
					/* The modem is ready at this moment in time, so
					 * send an event to the state machine.  
					 */
					setEventID(UCONN_POWERUP_RSP);
				}
				else 
				{
//					setEventID();
				}
				break;
			}
		}
	}
	else if((strstr(atCommandRsp.c_str(),"OK") || (strstr(atCommandRsp.c_str(),"ERROR"))))
	{
		// do nothing.
	}
	else
	{
		UBufferParser::processAtCommandResp(atCommandRsp);
	}

    //case DSFLOWRPT	
	/* Process the Response of DSFLORPT command recd from Modem. */

	//case CGDCONT
	/* Process the Response ofCGDCONT command recd from Modem. */
}



void UThreeGParser::processUnSolResp(string &unSolicitedRspStr)
{
	setEventID(0);
	vector<string> vStr;
	string tmpStr = "";
	int size = 0;

	//cout << "The unsolicited events received:" << unSolicitedRspStr << endl;
	
	if(strstr(unSolicitedRspStr.c_str(),"+CREG:") || (strstr(unSolicitedRspStr.c_str(),"+CGREG:")))
	{
		/* Check for +CEREG: and set the relevant Event */
		tokeniseInformation (unSolicitedRspStr,vStr, '\n' ) ;
		size = vStr.size();
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"+CREG:") || (strstr(vStr[i].c_str(),"+CGREG:")))
			{
				if(strstr(vStr[i].c_str(),","))
				{
					tmpStr.clear();
					tmpStr = vStr[i];
					vStr.clear();
					tokeniseInformation(tmpStr,vStr,',');
					int size = vStr.size();
					if(size >= 2)
					{
						tmpStr.clear();
						tmpStr = vStr[1];
						parsedInfo.setModem_State(atoi(tmpStr.c_str()));
						setEventID(UCONN_EVT_NM_MODEM_STATE_UPDATE);
						break;
					}
					break;
				}
				else
				{
					tmpStr.clear();
					tmpStr = vStr[i];
					vStr.clear();
					tokeniseInformation(tmpStr,vStr,':');
					tmpStr = vStr[1];
					parsedInfo.setModem_State(atoi(tmpStr.c_str()));
					setEventID(UCONN_EVT_NM_MODEM_STATE_UPDATE);
					break;
				}
			}
		}
	unSolicitedRspStr = "";
	return;
	}
	else if(strstr(unSolicitedRspStr.c_str(),"+CUSD:"))
	{
		if (strstr(unSolicitedRspStr.c_str(),","))
		{
			tokeniseInformation (unSolicitedRspStr, vStr, ',');
			size = vStr.size();
			for (int i = 1; i <= size; i++ )
			{
				tmpStr = vStr[i-1];
				if ( strstr(tmpStr.c_str(),"CUSD:"))
				{
					vStr.clear();
					int FirstIndex = i;
					tokeniseInformation (tmpStr, vStr, ':'); 
					if ( vStr.size() >= 2)
					{
						if ((atoi(vStr[1].c_str()) == 4))
						{
							char USSDStr[500];
							string ErrorMsg ="Operation not supported by Network";
							strncpy( USSDStr, ErrorMsg.c_str(), ErrorMsg.size());
							USSDStr[ErrorMsg.size()] = '\0';
							parsedInfo.setUSSDRsp(USSDStr);
							setEventID(UCONN_EVT_NM_USSD_RSP);
						}
						else if ((atoi(vStr[1].c_str()) == 5))
						{
							char USSDStr[500];
							string ErrorMsg ="Network time out";
							strncpy( USSDStr, ErrorMsg.c_str(), ErrorMsg.size());
							USSDStr[ErrorMsg.size()] = '\0';
							parsedInfo.setUSSDRsp(USSDStr);
							setEventID(UCONN_EVT_NM_USSD_RSP);
						}
						else
						{
							vStr.clear();
							tokeniseInformation (unSolicitedRspStr,vStr, ',' );
							size = vStr.size();

							if ( size >= 2 )
							{
								char USSDStr[500];
								replaceAll(vStr[FirstIndex],"\"","");
								strncpy( USSDStr, vStr[FirstIndex].c_str(), vStr[FirstIndex].size());
								USSDStr[vStr[FirstIndex].size()] = '\0';
								parsedInfo.setUSSDRsp(USSDStr);
								setEventID(UCONN_EVT_NM_USSD_RSP);
							}

							/*
							for ( int i = 0; i<= size; i++ )
							{
							if(strstr(vStr[i].c_str(),"+CUSD:") )
							{
							tmpStr.clear();
							tmpStr = vStr[i];
							vStr.clear();
							tokeniseInformation(tmpStr,vStr,',');
							int size = vStr.size();
							if(size >= 2)
							{
							tmpStr.clear();
							tmpStr = vStr[1];
							vStr.clear();
							tokeniseInformation(tmpStr,vStr,'"');

							char USSDStr[500];
							strncpy( USSDStr, vStr[1].c_str(), vStr[1].size());
							USSDStr[vStr[1].size()] = '\0';
							parsedInfo.setUSSDRsp(USSDStr);
							setEventID(UCONN_EVT_NM_USSD_RSP);
							break;
							}
							}
							}*/
						}
					}
				}
			}
		}
	}
    else if(strstr(unSolicitedRspStr.c_str(),"^BOOT:"))
	{
		// DO nothing, but this message keeps coming. 
	}
    else if(strstr(unSolicitedRspStr.c_str(),"^MODE:"))
	{
		/* Process the MODE command recd from Modem/Network. */
		/*
		^MODE: N1, N2
		N1:
		<sys_mode>: System mode. The values are as follows:
		0    No service.
		1    AMPS mode (not in use currently)
		2    CDMA mode (not in use currently)
		3    GSM/GPRS mode
		4    HDR mode
		5    WCDMA mode
		6    GPS mode
		N2: 
		<sys_submode>: System sub mode. The values are as follows:
		0    No service.
		1    GSM mode
		2    GPRS mode
		3    EDGE mode
		4    WCDMA mode
		5    HSDPA mode
		6	 HSUPA mode
		7    HSDPA mode and HSUPA mode  Attachment is the detailed AT command of our
		data card, but please pay attention the user must obey the NDA and cannot
		spread to the third party.
		*/
	}
	else if(strstr(unSolicitedRspStr.c_str(),"^RSSI:"))
	{
		tokeniseInformation (unSolicitedRspStr,vStr, '\n' ) ;
		size = vStr.size();
		for ( int i = 0; i<= size; i++ )
		{
			if(strstr(vStr[i].c_str(),"^RSSI:"))
			{
				/* Process the RSSI command recd from Modem, and Set the rssi in the ThreeG_NM_Info struct. */
				tokeniseInformation(unSolicitedRspStr,vStr,':');

				int size = vStr.size();
				if(size >= 1)
				{
					parsedInfo.setRSSI( atoi(vStr[1].c_str()));
					setEventID(UCONN_EVT_NM_RSSI_UPDATE);
					break;
				}
				break;
			}
		}
		vStr.clear();
	}
}
