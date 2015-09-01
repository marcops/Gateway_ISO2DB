#ifndef _XMLPROCESS_H_
#define _XMLPROCESS_H_

#include "defines.h"
#include "tinyXml/tinyxml.h"
#include "ProcessConfig.h"
#include "BitRow.h"
#include "GatewayInProcessConfig.h"

class XmlProcess {
public:
	XmlProcess();
	~XmlProcess();
	bool Load( const char* path );
	GatewayInProcessConfig* GetProcessConfig(){ return m_conf;}
private:
	GatewayInProcessConfig *m_conf;
	bool ReadFile( TiXmlDocument*  doc );
	bool ParserProc( TiXmlHandle *hRoot );
	void ParserProcess(TiXmlElement *process, ProcessConfig* proc );
	void ParserBitRows(TiXmlElement *bitrow , ProcessConfig *conf );



};

#endif