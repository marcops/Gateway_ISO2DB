#ifndef _GatewayInProcess_H_
#define _GatewayInProcess_H_

#include "defines.h"
#include "SocketServer.h"
#include <Queue>
#include "IniReader.h"
#include "GatewayInProperties.h"
#include "ProcessIso2DB.h"

#define THREADS_INPROCESS 4

class GatewayInProcess {
public:
	GatewayInProcess(SocketServer* skt);
	~GatewayInProcess();
	void Execute(SOCKET child);
	void ExecuteRecv();
	void ExecuteSend();
	void ExecuteProcessMessage();
	int GetSizeSendList();
	int GetSizeRecvList();
	void EndProcess(const char* msg );

	//private
	void ExecuteUpdateInterface();
	void InsertMessageToSend(string msg );
	void WaitAllThreads();
private:
	//server
	SocketServer *m_server;

	SOCKET m_ServerChild;	

	//for blocking Execute, free if conection is lost
	HANDLE m_RecvMutex;
	HANDLE m_RecvSem;
	HANDLE m_SendMutex;
	HANDLE m_SendSem;
	//
	HANDLE m_waitThread;
	//wait threads finish!
	HANDLE m_parallel[THREADS_INPROCESS];

	//Queue
	queue<string> m_QRecv;
	queue<string> m_QSend;
	
	bool g_running;

};


#endif