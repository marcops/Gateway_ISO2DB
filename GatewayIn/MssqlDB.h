#ifndef _MSSQLDB_H_
#define _MSSQLDB_H_
#include "defines.h"
#include "GatewayInProperties.h"
#import "C:\Arquivos de programas\Arquivos comuns\System\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")

class MssqlDB {
public:
	MssqlDB(string strConn);
	~MssqlDB();
	void StoreProcedure( const char* name );
	void InsertParameter(const char* name , const char* value, ParameterDirectionEnum sqlType);
	void Execute();
	VARIANT GetParameter(const char *name );
	bool IsConnect();
private:
	_ConnectionPtr pConn;
	_CommandPtr pCom;
	bool m_connected;

};

#endif