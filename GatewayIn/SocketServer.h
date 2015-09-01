#ifndef _SOCKETSERVER_H_
#define _SOCKETSERVER_H_
#include "defines.h"
#include "GatewayInProperties.h"

class SocketServer {
public:
	SocketServer(int port);
	~SocketServer();

	bool Send(SOCKET client, const char* msg, int len);
	bool Send(SOCKET client, const char* msg);
	bool Receive(SOCKET client, void *result , int *len);
	SOCKET Accept();
	void CloseClient( SOCKET client); 
	bool SetTimeout(SOCKET sock,int seconds );
private:
	int m_socket;
	int m_port;
	bool m_err;
	struct sockaddr_in m_local;
	struct sockaddr_in m_remote;

	bool InitServer();
};

#endif