#ifndef UVPN_H
#define UVPN_H

#include <string>

enum VPN_SET_PARAM {
	VPN_PROFILE=0

};

class UVpn {
private:

	static UVpn* uVpn;

	std::string vpnProfName;
	

	UVpn();
	~UVpn(){}

	

public:

	std::string vpnClientList;

	static UVpn* getVpn();

	std::string listVpnClients();
	
	std::string listVpnProfiles(); //list tokenized by '\n'

	void vpnConnect();

	void vpnDisconnect();

	void setVpnProfName(std::string vpnProfName);

	void setParam(unsigned int paramId,std::string param);

	
	

};
void vpn_init();
//void vpnConnectToServer();

#endif //UVPN_H