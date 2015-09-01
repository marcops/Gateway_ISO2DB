#ifndef _GATEWAYPROCESSDLG_H_
#define _GATEWAYPROCESSDLG_H_
#include "defines.h"

// GatewayProcessDlg dialog

class GatewayProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(GatewayProcessDlg)

public:
	GatewayProcessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GatewayProcessDlg();

// Dialog Data
	enum { IDD = IDD_LISTPROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedAddprocess();
};


#endif