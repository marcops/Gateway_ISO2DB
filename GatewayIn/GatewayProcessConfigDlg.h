#ifndef _GATEWAYPROCESSCONFIGDLG_H_
#define _GATEWAYPROCESSCONFIGDLG_H_

#include "defines.h"
// GatewayProcessConfigDlg dialog

class GatewayProcessConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(GatewayProcessConfigDlg)

public:
	GatewayProcessConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GatewayProcessConfigDlg();

// Dialog Data
	enum { IDD = IDD_PROCESSCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

#endif