#include "GatewayControlProcess.h"
//TODO memory leaks
//TODO fazer config do xml pela interface
//TODO Verificar possiveis erros do usuario
GatewayControlProcess::GatewayControlProcess(){	
	m_localPort = GatewayInProperties::getInstance()->ServerPort();
	m_server = new SocketServer(m_localPort);
	m_gtProc = NULL;
	m_end = false;
	m_accept = CreateEvent( NULL , FALSE , FALSE , NULL );
}
GatewayControlProcess::~GatewayControlProcess(){
	//accept kill?
	if(m_gtProc!=NULL) {
		m_end = true;
		m_gtProc->EndProcess("");
		WaitForSingleObject(m_accept, INFINITE);
		CloseHandle(m_accept);
	}
	delete m_server;

}
void GatewayControlProcess::ExecuteControl(){
	while( true ) {
		m_gtProc = new GatewayInProcess( m_server);
		_SHOWMSG( "Waiting a client..." )
		SOCKET child = m_server->Accept();
		if( child ) m_gtProc->Execute(child);
		
		delete m_gtProc;
		if( m_end ) break;
	}
	SetEvent(m_accept);
}
static DWORD WINAPI ExecuteProcessThread(LPVOID param){
	GatewayControlProcess *ptr = (GatewayControlProcess*)param;
	ptr->ExecuteControl();	
	return 0;
}


void GatewayControlProcess::Execute(){
	CreateThread(NULL, 0, ExecuteProcessThread, this, 0, 0);	
}