#ifndef _GATEWAYINPROCESSCONFIG_H_
#define _GATEWAYINPROCESSCONFIG_H_

#include "defines.h"
#include "ProcessConfig.h"

class GatewayInProcessConfig {
public:
	~GatewayInProcessConfig();
	void ProcConfig( list<ProcessConfig*>& x){ m_processConf=x;}
	list<ProcessConfig*>& ProcConfig(){ return m_processConf;}
	ProcessConfig* FindProcessConfig( const char* msgClass , const char* processCode );
private:
	 list<ProcessConfig*> m_processConf;
};

#endif