#ifndef _PROCESSCONFIG_H_
#define _PROCESSCONFIG_H_
#include "defines.h"
#include "BitRow.h"

class ProcessConfig{
public:
	~ProcessConfig();
	string Tag(){ return m_tag;}
	string Table(){ return m_table;}
	string MessageClass(){ return m_messageClass;}
	string ProcessCode(){ return m_processCode;}
	string StoredProcedure() { return m_storedProcedure; }
	list<BitRow*>& LstBitRow(){ return m_process;}

	void Tag(string x) { m_tag=x;}
	void Table(string x) { m_table=x;}
	void MessageClass(string x) { m_messageClass=x;}
	void ProcessCode(string x) { m_processCode=x;}
	void StoredProcedure(string x){ m_storedProcedure=x;}
	void LstBitRow(list<BitRow*> x) { m_process=x;}
private:
	string m_tag;
	string m_table;
	string m_messageClass;
	string m_processCode;
	string m_storedProcedure;
	list<BitRow*> m_process;
};
#endif