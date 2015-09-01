#include "ProcessConfig.h"

ProcessConfig::~ProcessConfig(){
	list<BitRow*>::iterator it;
	for ( it=m_process.begin() ; it != m_process.end(); it++ ){
		delete (*it);
	}
}