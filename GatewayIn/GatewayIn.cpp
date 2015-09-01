#include "GatewayIn.h"



/*#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#endif*/

CGatewayInApp theApp;

CGatewayInApp::CGatewayInApp(){}

BOOL CGatewayInApp::InitInstance(){

	//_CrtSetDbgFlag (   _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF |_CRTDBG_DELAY_FREE_MEM_DF |_CRTDBG_LEAK_CHECK_DF );
	//_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG);

	m_inter = new GatewayInDlg();
	GatewayInProperties::getInstance()->SetInterface(m_inter);
	_SHOWMSG("Loading Configuration...")
		XmlProcess *m_xml = new XmlProcess();
	//verify XML
	if( !m_xml->Load(GatewayInProperties::getInstance()->XmlConfig().c_str()) ) {
		_SHOWMSG("Configure your XML and restart!")
			//waiting client close this view!
			m_inter->DoModal();
		delete m_inter;
		delete m_xml;
		return TRUE;
		}	
	GatewayInProperties::getInstance()->ProcessConfig(m_xml->GetProcessConfig());

	_SHOWMSG("Configuration Loaded!")
		//verify INI
		if( !PathFileExists( INI_FILE )){
			_SHOWMSG("Configure your INI file and restart!")
				//waiting client close this view!
				m_inter->DoModal();
			delete m_xml;
			delete m_inter;
			return TRUE;
			}
		MssqlDB *db = new MssqlDB(GatewayInProperties::getInstance()->StrConnectionDB());
		bool connected = db->IsConnect();
		delete db;
		if( !connected ) {
			_SHOWMSG("Verify your Database and restart!")
				//waiting client close this view!
				m_inter->DoModal();
			delete m_xml;
			delete m_inter;
			return TRUE;
			}

		GatewayControlProcess* gtProc = new GatewayControlProcess();
		gtProc->Execute();

		//waiting client close this view!
		m_inter->DoModal();
		delete m_xml;
		delete GatewayInProperties::getInstance();
		delete gtProc;
		delete m_inter;	

		_CrtDumpMemoryLeaks();
		

		return TRUE;
	}


