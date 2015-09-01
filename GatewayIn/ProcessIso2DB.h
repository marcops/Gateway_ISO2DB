#ifndef _PROCESSISO2DB_H_
#define _PROCESSISO2DB_H_
#include "defines.h"
#include "iso8583.h"
#include "Mssqldb.h"

class ProcessIso2DB {
public:
	ProcessIso2DB ( void *parent , string read );
	bool Execute(char *send);
	void *Parent();
private:
	void *m_parent;
	string m_read;

	string Variant2String(VARIANT& vt);
	void RemoveSpace(string &toRemove);
};

#endif