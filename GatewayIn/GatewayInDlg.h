#ifndef _GATEWAYINDLG_H_
#define _GATEWAYINDLG_H_

#include "defines.h"
#include "afxwin.h"
#include "GatewayInProperties.h"

class GatewayInProperties;
class GatewayInDlg : public CDialog {
	DECLARE_DYNAMIC(GatewayInDlg)
public:
	GatewayInDlg(CWnd* pParent = NULL);
	~GatewayInDlg();
	enum { IDD = IDD_GTWININTERFACE};
	//n bloqueante
	void ShowMessage(string msg);
	void ShowTPIn(string msg);
	void ShowTPOut(string msg);
	void ShowNProc(string msg);
	//Bloqueante
	void InsertMsg( string msg );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedConfig();
	CListBox m_lstLog;

	CSemaphore *m_sem;

private:
	CEdit m_nProcess;
	CEdit m_transIn;
	CEdit m_transOut;
};

class ThreadMsg {
public:
	GatewayInDlg* m_cl;
	string m_msg;
};

#endif