#include "MssqlDB.h"

MssqlDB::MssqlDB(string strCon){
	HRESULT hr = S_OK;
	m_connected = false;
	//Initialize the COM Library
	CoInitialize(NULL);

	try
	{
		//Create the Connection pointer
		hr = pConn.CreateInstance((__uuidof(Connection)));
		if(FAILED(hr))
		{
			//_SHOWMSG("Error instantiating Connection object")
			CoUninitialize();
			return;
		}

		//Open the SQL Server connection
		hr = pConn->Open(strCon.c_str(),"","",0);
		if(FAILED(hr))
		{
			//_SHOWMSG("Error Opening Database object")
			CoUninitialize();
			return;
		}

		//Create the C++ ADO Command Object
		pCom.CreateInstance(__uuidof(Command));
		pCom->ActiveConnection = pConn;
		m_connected = true;
	} catch(...)	{

		/*_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		char tmp[500];		
		sprintf(tmp,"\n Source : %s \n Description : %s \n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription);
		_SHOWMSG(tmp)*/

	}

}

void MssqlDB::StoreProcedure( const char* name ) {
	//Make the ADO C++ command object to accept stored procedure
	pCom->CommandType = adCmdStoredProc ;
	//Tell the name of the Stored Procedure to the command object
	pCom->CommandText = _bstr_t(name);
}
void MssqlDB::InsertParameter(const char* name , const char* value, ParameterDirectionEnum sqlType){
	VARIANT varAdd;
	varAdd.vt = VT_BSTR; //Variant type for BSTR
	varAdd.bstrVal = _bstr_t(value);

	pCom->Parameters->Append(pCom->CreateParameter(_bstr_t(name),adChar,sqlType,50,varAdd));
}
void MssqlDB::Execute(){
	pCom->Execute(NULL,NULL,adCmdStoredProc);
}
bool MssqlDB::IsConnect(){
	return m_connected;
}
VARIANT MssqlDB::GetParameter(const char *name ){
	_ParameterPtr pt = pCom->Parameters->GetItem(name);
	return  pt->GetValue();
}

MssqlDB::~MssqlDB(){
	pConn->Close();
	CoUninitialize();
}