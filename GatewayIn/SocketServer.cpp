#include "SocketServer.h"


SocketServer::SocketServer(int port){
	m_socket = 0;
	m_port = port;
	m_err = false;

	m_err = !InitServer();
}

bool SocketServer::SetTimeout(SOCKET sock,int seconds ){
	/*
	flag = fcntl( *psocket, F_GETFL, 0 );
fcntl( *psocket, F_SETFL, flag | O_NONBLOCK );

	*/
	struct timeval tv; 
	tv.tv_sec = seconds;
	tv.tv_usec = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,  sizeof(tv) ) )
		return false;
	return true;

}

bool SocketServer::InitServer(){
	WSADATA wsaData;

	if(WSAStartup(MAKEWORD(2,1),&wsaData) != 0 ){
		_SHOWMSG( "cannot init socket lib - version wrong?" )
		return false;
	}

	m_socket = socket(AF_INET,SOCK_STREAM,0);
	if(m_socket == INVALID_SOCKET) {
		_SHOWMSG( "cannot open socket" )
		return false;
	}



	m_local.sin_family=AF_INET; //Address family
	m_local.sin_addr.s_addr=INADDR_ANY; //Wild card IP address
	m_local.sin_port=htons((u_short)m_port); //port to use

	if(bind(m_socket,(sockaddr*)&m_local,sizeof(m_local)) != 0) {
		_SHOWMSG( "cannot bind socket" )
		return false;
	}
	SetTimeout(m_socket,2147483647);
	if(listen(m_socket,20) != 0){
		_SHOWMSG( "cannot listen socket - Max 20 connection" )
		return false;
	}

	return true;
}

SOCKET SocketServer::Accept(){
	int len_remote = sizeof(m_remote);
	SOCKET socketClient = accept(m_socket, (struct sockaddr*)&m_remote, &len_remote);
	//_SHOWMSG( "Connection from " << inet_ntoa(m_remote.sin_addr) << endl;
	return socketClient;
}

void SocketServer::CloseClient( SOCKET client ){
	closesocket( client );
}
SocketServer::~SocketServer(){
	if(!m_err){
		closesocket(m_socket);
		WSACleanup();
	}
}


bool SocketServer::Receive(SOCKET client,void *result , int *len){
	*len = recv( client , (char*)result , *len , 0);
	if(*len<0) {
		_SHOWMSG( "cannot receive data" )
		closesocket(client);
		return false;
	}
	if( *len ==0 ){
		_SHOWMSG("Connection lost")
		closesocket(client);
		return false;
	}
	return true;
}
bool SocketServer::Send(SOCKET client,const char* msg){
	return this->Send( client,msg, strlen(msg) );
}

bool SocketServer::Send(SOCKET client,const char* msg, int len){
	int rc = send(client, msg, len,0);
	if(rc<0) {
		_SHOWMSG( "cannot send data" )
		closesocket(client);
		return false;
	}
	return true;
}