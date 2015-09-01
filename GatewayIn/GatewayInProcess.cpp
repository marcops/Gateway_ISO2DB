#include "GatewayInProcess.h"

static volatile int g_nThreads;

void GatewayInProcess::WaitAllThreads(){
//	m_waitThread = CreateEvent( NULL , FALSE , FALSE , NULL );
	//wait all threads!
	WaitForMultipleObjects( THREADS_INPROCESS , m_parallel , true, INFINITE );
	for( int i = 0 ; i < THREADS_INPROCESS; i++ ) {
		ReleaseMutex(m_parallel[i]);
	}
//	WaitForSingleObject(m_waitThread , INFINITE );
//	CloseHandle(m_waitThread);
}

GatewayInProcess::~GatewayInProcess(){
	CloseHandle(m_RecvMutex);
	CloseHandle(m_SendMutex);
	CloseHandle(m_RecvSem);
	CloseHandle(m_SendSem);

}
GatewayInProcess::GatewayInProcess( SocketServer* skt){
	m_server = skt;
	g_running = true;
	g_nThreads = 1;

	m_RecvMutex = CreateMutex(NULL, false, NULL);
	m_RecvSem = CreateSemaphore(NULL,0,1,NULL);

	m_SendMutex = CreateMutex(NULL, false, NULL);
	m_SendSem = CreateSemaphore(NULL,0,1,NULL);


	//interface
	m_parallel[0] = CreateEvent( NULL , FALSE , FALSE , NULL );
	//processMessage
	m_parallel[1] = CreateEvent( NULL , FALSE , FALSE , NULL );
	//send
	m_parallel[2] = CreateEvent( NULL , FALSE , FALSE , NULL );
	//recv
	m_parallel[3]= CreateEvent( NULL , FALSE , FALSE , NULL );

}

void GatewayInProcess::ExecuteUpdateInterface(){
	char msg[100];

	while( true ) {
		if( !g_running ) break;
		sprintf(msg , "%d" , this->GetSizeRecvList() );
		GatewayInProperties::getInstance()->ShowTPIn(msg);

		sprintf(msg , "%d" , this->GetSizeSendList() );
		GatewayInProperties::getInstance()->ShowTPOut(msg);

		sprintf(msg , "%d" , g_nThreads );		
		GatewayInProperties::getInstance()->ShowNProc(msg);

		Sleep(1000);
	}
	EndProcess( "" );
	SetEvent(m_parallel[0]);

}

int GatewayInProcess::GetSizeRecvList(){
	return m_QRecv.size();
}

int GatewayInProcess::GetSizeSendList(){
	return m_QSend.size();
}

void GatewayInProcess::EndProcess(const char* msg ){
	if( msg != 0 ) {
		if( strlen(msg) > 0 ) 
			_SHOWMSG(msg)
	}
	g_running = false;
	ReleaseSemaphore(m_RecvSem,1, NULL);
	ReleaseSemaphore(m_SendSem,1, NULL);
	g_nThreads = 0;
}

static DWORD WINAPI ExecuteSendThread(LPVOID param){
	GatewayInProcess *ptr = (GatewayInProcess*)param;
	ptr->ExecuteSend();	
	return 0;
}
static DWORD WINAPI ExecuteProcessMessageThread(LPVOID param){
	GatewayInProcess *ptr = (GatewayInProcess*)param;
	ptr->ExecuteProcessMessage();	
	return 0;
}

static DWORD WINAPI ExecuteProcessIso2DBThread(LPVOID param){
	g_nThreads++;
	ProcessIso2DB *proc = (ProcessIso2DB*)param;
	char send[LEN_MAX_BIT];
	memset( send , 0 , sizeof(send ));
	proc->Execute(send);	
	((GatewayInProcess*)proc->Parent())->InsertMessageToSend(send);
	delete proc;
	g_nThreads--;
	return 0;
}
static DWORD WINAPI ExecuteRecvThread(LPVOID param){
	GatewayInProcess *ptr = (GatewayInProcess*)param;
	ptr->ExecuteRecv();	
	return 0;
}

void GatewayInProcess::InsertMessageToSend(string msg ){
	
	try{
		char pack[LEN_MAX_BIT];
		memset( pack , 0 , sizeof(pack));
		sprintf( pack , "%04d%s" , msg.length() , msg.c_str());
		WaitForSingleObject(m_SendMutex, INFINITE); //Mutex Lock
		if( this && !g_running ) return;
		m_QSend.push( pack );
		ReleaseSemaphore(m_SendSem,1, NULL);
		ReleaseMutex(m_SendMutex);
	} catch(...){
		//conexao encerrada no meio.

	}
	
}
//ExecuteProcessMEssage
void GatewayInProcess::ExecuteProcessMessage(){
	string read;
	while(true){
		if( !g_running ) break;
		WaitForSingleObject(m_RecvSem,INFINITE); //Wait for producer;
		WaitForSingleObject(m_RecvMutex, INFINITE); //Mutext Lock
		while(m_QRecv.size()>0){
			read = m_QRecv.front();
			m_QRecv.pop();
			
			ProcessIso2DB *proc = new ProcessIso2DB(this, read);
			CreateThread(NULL, 0, ExecuteProcessIso2DBThread, proc, 0, 0);

		}
		ReleaseMutex(m_RecvMutex); //Mutex Unlock
	}
	//ReleaseMutex(m_RecvMutex); //Mutex Unlock
	EndProcess( "" );
	//escondido no ExecuteProcessIso2DBThread
	SetEvent(m_parallel[1]);

}
void GatewayInProcess::ExecuteSend(){
	string read;

	while( true ) {
		if( !g_running ) break;
		WaitForSingleObject(m_SendSem,INFINITE); //Wait for producer;
		WaitForSingleObject(m_SendMutex, INFINITE); //Mutext Lock
		while(m_QSend.size()>0){
			read = m_QSend.front();
			m_QSend.pop();
			if( !m_server->Send(m_ServerChild , read.c_str() , read.length()) ){
				EndProcess("Message cannot be sended");
				SetEvent(m_parallel[2]);
				ReleaseMutex(m_SendMutex); //Mutex Unlock
				return;
			}
		}
		ReleaseMutex(m_SendMutex); //Mutex Unlock
	}
	EndProcess("");
	SetEvent(m_parallel[2]);
}

void GatewayInProcess::ExecuteRecv(){
	char received[RECEIVE_MAX_MSG];
	char lenRead[HEADER_LEN+1]; 
	int lenght_received;
	string read;

	while( true ){
		lenght_received = HEADER_LEN;
		memset( lenRead , 0 , sizeof(lenRead));
		if(!m_server->Receive(m_ServerChild, lenRead, &lenght_received) ) {
			EndProcess( "ExecuteRecv ERR: 0" );
			SetEvent(m_parallel[3]);
			return;
		}
		if( lenght_received != 4 ) { 
			EndProcess( "read Lenght diff from ExecuteRecv" );
			SetEvent(m_parallel[3]);
			return;
		}
		lenght_received = atoi(lenRead);
		memset( received , 0 , sizeof(received));
		if(!m_server->Receive(m_ServerChild, received, &lenght_received) ) {
			EndProcess( "ExecuteRecv  ERR: 1" );
			SetEvent(m_parallel[3]);
			return;
		}

		read = lenRead;
		read += received;
		if( !g_running ) break;
		WaitForSingleObject(m_RecvMutex, INFINITE); //Mutex Lock
		m_QRecv.push( read );
		ReleaseSemaphore(m_RecvSem,1, NULL);
		ReleaseMutex(m_RecvMutex); //Mutex Unlock

	}
	EndProcess( "" );
	//ReleaseSemaphore(m_RecvSem,1, NULL);
	SetEvent(m_parallel[3]);

}
static DWORD WINAPI ExecuteUpdateInterfaceThread(LPVOID param){
	GatewayInProcess *ptr = (GatewayInProcess*)param;
	ptr->ExecuteUpdateInterface();	
	return 0;
}

/*****************************************************************
|
|
T|---Recv GetNet ----> Put Queue Recv
H|     ^___________________|
R|
E|--Get Queue Recv-¬--> Send DB --> Recv DB --> Put Queue Send
A|     ^__thread___|
D|
|--Get Queue Send ----> Send GetNet
|        ^_________________|
|
******************************************************************/
void GatewayInProcess::Execute(SOCKET child){
	m_ServerChild = child;
	_SHOWMSG( "Client Received" )


	CreateThread(NULL, 0, ExecuteRecvThread, this, 0, 0);
	CreateThread(NULL, 0, ExecuteProcessMessageThread, this, 0, 0);
	CreateThread(NULL, 0, ExecuteSendThread, this, 0, 0);
	CreateThread(NULL, 0, ExecuteUpdateInterfaceThread, this, 0, 0);	
	WaitAllThreads();
	
}