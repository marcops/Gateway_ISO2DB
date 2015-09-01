#include "GatewayInDlg.h"
#include "GatewayProcessDlg.h"

BEGIN_MESSAGE_MAP(GatewayInDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &GatewayInDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CONFIG, &GatewayInDlg::OnBnClickedConfig)
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(GatewayInDlg, CDialog)
GatewayInDlg::GatewayInDlg(CWnd* pParent /*=NULL*/) : CDialog(GatewayInDlg::IDD){
	m_sem = new CSemaphore(1,1,0,0);
	m_sem->Lock();	

}
GatewayInDlg::~GatewayInDlg(){
	delete m_sem;
}
void GatewayInDlg::ShowTPIn(string msg){
	m_transIn.SetWindowText( msg.c_str());
}
void GatewayInDlg::ShowTPOut(string msg){
	m_transOut.SetWindowText( msg.c_str());
}

void GatewayInDlg::ShowNProc(string msg){
	m_nProcess.SetWindowText( msg.c_str());
}

void GatewayInDlg::InsertMsg( string msg ){
	m_sem->Lock();	
	if( !this ) return;
	if(m_lstLog.GetCount() >= 100 ) m_lstLog.DeleteString(100);
	m_lstLog.InsertString(0,msg.c_str());
	m_sem->Unlock();
}

static DWORD WINAPI ExecuteInsertMsg(LPVOID param){
	ThreadMsg *nmsg = (ThreadMsg*)param;
	nmsg->m_cl->InsertMsg(nmsg->m_msg);	
	delete nmsg;
	return 0;
}

void GatewayInDlg::ShowMessage(string msg){
	//hey isso vai virar uma fila de thread! cool!!!!
	// eu gosto do que faço :D
	ThreadMsg* nmsg = new ThreadMsg();
	nmsg->m_cl = this;
	nmsg->m_msg = msg;
	CreateThread(NULL, 0, ExecuteInsertMsg, nmsg, 0, 0);
}

BOOL GatewayInDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SERVER));
	this->SetIcon(hIcon, true);
	m_sem->Unlock();
	return true;
}
void GatewayInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTLOG, m_lstLog);
	DDX_Control(pDX, IDC_NPROC, m_nProcess);
	DDX_Control(pDX, IDC_TPSIN, m_transIn);
	DDX_Control(pDX, IDC_TPSOUT, m_transOut);
}
void GatewayInDlg::OnBnClickedCancel(){
	GatewayInProperties::getInstance()->SetInterface(NULL);
	OnCancel();
}

void GatewayInDlg::OnBnClickedConfig()
{
	::MessageBox(0 , "Not Yet!" , "Alert" , 0 );
	//GatewayProcessDlg* procDlg = new GatewayProcessDlg();
	//procDlg->DoModal();
	//delete procDlg;
}

