#include "GatewayInProperties.h"

GatewayInProcessConfig* GatewayInProperties::ProcessConfig(  ){
	return m_processConfig;
}

int GatewayInProperties::ServerPort(){
	return m_localPort;
}
string GatewayInProperties::StrConnectionDB(){
	return m_strConnection;
}

GatewayInProperties::GatewayInProperties(){
	CIniReader iniReader(INI_FILE);	
	m_localPort = iniReader.Read("SERVER", "port", 0);   
	m_strConnection = iniReader.Read("DATABASE", "strConnection", ""); 
}
void GatewayInProperties::ProcessConfig( GatewayInProcessConfig* proc ){
	m_processConfig = proc;
}
GatewayInProperties* GatewayInProperties::getInstance() {
	static GatewayInProperties *instance = 0;
	if (!instance) {
		instance = new GatewayInProperties();
		instance->SetInterface(NULL);
		instance->XmlConfig("gtwInProcess.xml");
	}
	return instance;
}
void GatewayInProperties::ShowMessage(string msg){
	if(m_interface != NULL)m_interface->ShowMessage(msg);
}
void GatewayInProperties::ShowTPIn(string msg){
	if(m_interface != NULL)m_interface->ShowTPIn(msg);
}
void GatewayInProperties::ShowTPOut(string msg){
	if(m_interface != NULL)m_interface->ShowTPOut(msg);
}
void GatewayInProperties::ShowNProc(string msg){
	if(m_interface != NULL)m_interface->ShowNProc(msg);
}
void GatewayInProperties::SetInterface( GatewayInDlg* inter){
	m_interface = inter;
}
void GatewayInProperties::XmlConfig(string xml ){
	m_xmlConfig = xml;
}
string GatewayInProperties::XmlConfig( ){
	return m_xmlConfig;
}