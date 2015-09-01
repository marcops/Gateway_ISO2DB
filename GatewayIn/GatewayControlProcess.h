#ifndef _GATEWAYCONTROLPROCESS_H_
#define _GATEWAYCONTROLPROCESS_H_

#include "defines.h"
#include "SocketServer.h"

#include "GatewayInProcess.h"

class GatewayControlProcess {
private:
	//server
	SocketServer *m_server;	
	int m_localPort;
	GatewayInProcess *m_gtProc;
	bool m_end;
	HANDLE m_accept;
public:
	GatewayControlProcess();
	~GatewayControlProcess();
	void Execute();
	//private
	void ExecuteControl();

};
#endif