#include "ProcessIso2DB.h"

ProcessIso2DB::ProcessIso2DB( void *parent , string read ){
	m_parent = parent;	
	m_read = read;
}
void *ProcessIso2DB::Parent(){
	return m_parent;
}

string ProcessIso2DB::Variant2String(VARIANT& vt){
	_bstr_t bs(vt);
	return string(static_cast<const char*>(bs));
}

void ProcessIso2DB::RemoveSpace(string &toRemove){
	char newValue[LEN_MAX_BIT];
	memset(newValue,0,sizeof(newValue));

	strncpy(newValue , toRemove.c_str() , toRemove.size() );
	for( int i= toRemove.size()-1; i >0; i-- ){
		if( newValue[i] == ' ' ) newValue[i] = 0;
		else break;
	}
	toRemove = newValue;

}

bool ProcessIso2DB::Execute(char *send){
	char bit[LEN_MAX_BIT];
	char msgClass[4+1], procCode[6+1];

	Iso8583 *iso = new Iso8583(ISO_IPIRANGA);
	iso->Extract( m_read.c_str()+HEADER_LEN);

	GatewayInProcessConfig* proc = GatewayInProperties::getInstance()->ProcessConfig();

	memset(msgClass,0,sizeof(msgClass));
	memset(procCode,0,sizeof(procCode));
	strncpy(msgClass, m_read.c_str()+HEADER_LEN  , 4 );
	iso->Get(3,procCode );

	ProcessConfig* process = proc->FindProcessConfig(msgClass , procCode );
	if( process ) {
		MssqlDB *db = new MssqlDB(GatewayInProperties::getInstance()->StrConnectionDB());
		if( db->IsConnect() ) {
			db->StoreProcedure(process->StoredProcedure().c_str());		

			list<BitRow*>::iterator it;
			for ( it=process->LstBitRow().begin() ; it != process->LstBitRow().end(); it++ ){
				memset(bit , 0 , sizeof(bit));
				iso->Get(atoi((*it)->Bit().c_str()),bit );
				db->InsertParameter((*it)->SqlRow().c_str() , bit, (ParameterDirectionEnum)atoi((*it)->SqlType().c_str()));			
			}

			db->Execute();
			{
				char codProc[4+1];
				memset(codProc,0,sizeof(codProc));
				strncpy(codProc , m_read.c_str()+HEADER_LEN , 4 );
				codProc[2]='1';
				iso->CreateSend( send, codProc);
			}
			for ( it=process->LstBitRow().begin() ; it != process->LstBitRow().end(); it++ ){
				string value = Variant2String(db->GetParameter((*it)->SqlRow().c_str()) );
				RemoveSpace( value );
				iso->Set( atoi( (*it)->Bit().c_str() ), value.c_str() );
			}	
			delete db;
			delete iso;
			return true;
		} 
		delete db;
	}
	delete iso;
	return false;

}