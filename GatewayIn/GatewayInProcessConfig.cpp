#include "GatewayInProcessConfig.h"

GatewayInProcessConfig::~GatewayInProcessConfig(){
	list<ProcessConfig*>::iterator it;
	for ( it=m_processConf.begin() ; it != m_processConf.end(); it++ ){
		delete (*it);
	}
}
ProcessConfig* 
GatewayInProcessConfig::FindProcessConfig( const char* msgClass , const char* processCode ){
	list<ProcessConfig*>::iterator it;

	for ( it=m_processConf.begin() ; it != m_processConf.end(); it++ ){
		if( (*it)->MessageClass().compare(msgClass) == 0 ){
			if( (*it)->ProcessCode().compare(processCode) == 0 )
				return *it;
		}
	}
	return 0;
}