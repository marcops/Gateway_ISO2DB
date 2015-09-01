#ifndef _GATEWAYIN_H_
#define _GATEWAYIN_H_

#include "defines.h"
#include "GatewayInProcess.h"
#include "GatewayInDlg.h"
#include "GatewayInProperties.h"
#include "GatewayControlProcess.h"
#include "XmlProcess.h"
#include "MssqlDB.h"
#include "IniReader.h"

class CGatewayInApp : public CWinApp {
public:
	CGatewayInApp();

	virtual BOOL InitInstance();
private:
	GatewayInDlg *m_inter;
};

extern CGatewayInApp theApp;

#endif