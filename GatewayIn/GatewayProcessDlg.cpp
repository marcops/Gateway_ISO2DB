// GatewayProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GatewayIn.h"

#include "GatewayProcessDlg.h"
#include "GatewayProcessConfigDlg.h"


// GatewayProcessDlg dialog

IMPLEMENT_DYNAMIC(GatewayProcessDlg, CDialog)

GatewayProcessDlg::GatewayProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GatewayProcessDlg::IDD, pParent)
{

}

GatewayProcessDlg::~GatewayProcessDlg()
{
}

void GatewayProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// GatewayProcessDlg message handlers
BEGIN_MESSAGE_MAP(GatewayProcessDlg, CDialog)
	ON_BN_CLICKED(IDC_ADDPROCESS, &GatewayProcessDlg::OnBnClickedAddprocess)
END_MESSAGE_MAP()

void GatewayProcessDlg::OnBnClickedAddprocess()
{
	//GatewayProcessConfigDlg* confDlg = new GatewayProcessConfigDlg();
	//confDlg->DoModal();
	//delete confDlg;
}
