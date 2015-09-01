#include "xmlProcess.h"

XmlProcess::XmlProcess(){
	m_conf = new GatewayInProcessConfig();
}
XmlProcess::~XmlProcess(){
	delete m_conf;
}
void XmlProcess::ParserBitRows(TiXmlElement *bitrow , ProcessConfig *conf){
	list<BitRow*> lst;

	TiXmlElement* pElem =bitrow->FirstChildElement();
	for( pElem; pElem; pElem=pElem->NextSiblingElement()) {
		TiXmlElement* el =TiXmlHandle(pElem).FirstChildElement().Element();
		
		BitRow *bit = new BitRow();
		for( el; el; el=el->NextSiblingElement()) {
			const char *pKey=el->Value();
			if( strncmp(pKey, "Bit" , strlen("Bit") ) == 0) {bit->Bit(el->GetText()); continue; }
			if( strncmp(pKey, "SqlRow" , strlen("SqlRow") ) == 0){ bit->SqlRow(el->GetText());continue; }
			if( strncmp(pKey, "SqlType" , strlen("SqlType") ) == 0) {bit->SqlType(el->GetText());continue; }
		}
		lst.push_back( bit );

	}
	conf->LstBitRow( lst );
}

void XmlProcess::ParserProcess(TiXmlElement *process , ProcessConfig *proc ){
	TiXmlElement* pElem = process->FirstChildElement();
	for( pElem; pElem; pElem=pElem->NextSiblingElement()) {
		const char *pKey=pElem->Value();

		if( strncmp(pKey, "Tag" , strlen("Tag") ) == 0) {proc->Tag(pElem->GetText()); continue; }
		if( strncmp(pKey, "Table" , strlen("Table") ) == 0){ proc->Table(pElem->GetText());continue; }
		if( strncmp(pKey, "MessageClass" , strlen("MessageClass") ) == 0) {proc->MessageClass(pElem->GetText());continue; }
		if( strncmp(pKey, "ProcessCode" , strlen("ProcessCode") ) == 0){ proc->ProcessCode(pElem->GetText());continue; }
		if( strncmp(pKey, "StoredProcedure" , strlen("StoredProcedure") ) == 0){ proc->StoredProcedure(pElem->GetText());continue; }

		if( strncmp(pKey, "BitRows" , strlen("BitRows") ) == 0){
			ParserBitRows( pElem, proc );
		}
	}
} 
bool XmlProcess::ParserProc( TiXmlHandle *hRoot ){
	TiXmlElement* pElem;
	list<ProcessConfig*> lst;
	pElem=hRoot->FirstChild( "Proc" ).FirstChild().Element();
	for( pElem; pElem; pElem=pElem->NextSiblingElement()) {
		const char *pKey=pElem->Value();
		ProcessConfig* proc = new ProcessConfig();
		ParserProcess( pElem , proc);
		lst.push_back( proc );
	}
	m_conf->ProcConfig(lst);
	return true;
} 
bool XmlProcess::ReadFile( TiXmlDocument *doc ){
	TiXmlHandle hDoc(doc);	
	TiXmlHandle hRoot(0);
	hRoot=TiXmlHandle(hDoc.FirstChildElement().Element());
	return ParserProc( &hRoot);
}
bool XmlProcess::Load( const char* path ){
	TiXmlDocument doc(path);
	if(doc.LoadFile()) {
		if( ReadFile(&doc) )
			return true;
	}
	return false;
}