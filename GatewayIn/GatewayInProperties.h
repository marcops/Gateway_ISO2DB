#ifndef _GATEWAYINPROPERTIES_H_
#define _GATEWAYINPROPERTIES_H_

#include "defines.h"
#include "GatewayInDlg.h"
#include "GatewayInProcessConfig.h"
#include "IniReader.h"

class GatewayInDlg;

class GatewayInProperties {
private:
	GatewayInProperties();
	//xml config
	GatewayInDlg *m_interface;
	string m_xmlConfig;
	GatewayInProcessConfig *m_processConfig;
	//ini file
	int m_localPort;
	string m_strConnection;
	
public:
	static GatewayInProperties* getInstance();
	
	//interface
	void ShowMessage(string msg);	
	void ShowTPIn(string msg);
	void ShowTPOut(string msg);
	void ShowNProc(string msg);
	//xml
	void SetInterface( GatewayInDlg* inter);
	void XmlConfig(string xml );
	string XmlConfig( );
	void ProcessConfig( GatewayInProcessConfig* proc );
	GatewayInProcessConfig* ProcessConfig(  );
	//ini file
	int ServerPort();
	string StrConnectionDB();
};
#define _SHOWMSG(x) GatewayInProperties::getInstance()->ShowMessage(x);
#endif