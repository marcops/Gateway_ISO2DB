#include "IniReader.h"
#include <iostream>
#include <Windows.h>

CIniReader::CIniReader(char* szFileName){
	memset(m_szFileName, 0x00, 255);
	memcpy(m_szFileName, szFileName, strlen(szFileName));
}
int CIniReader::Read(char* szSection, char* szKey, int iDefaultValue){
	int iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName); 
	return iResult;
}
double CIniReader::Read(char* szSection, char* szKey, float fltDefaultValue){
	char szResult[255];
	char szDefault[255];
	double fltResult;

	GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName); 
	fltResult =  atof(szResult);
	return fltResult;
}
bool CIniReader::Read(char* szSection, char* szKey, bool bolDefaultValue){
	char szResult[255];
	char szDefault[255];
	bool bolResult;

	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
	bolResult =  (strcmp(szResult, "True") == 0 || 
		strcmp(szResult, "true") == 0) ? true : false;
	return bolResult;
}
char* CIniReader::Read(char* szSection, char* szKey, const char* szDefaultValue){
	static char szResult[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileString(szSection,  szKey, 
		szDefaultValue, szResult, 255, m_szFileName); 
	return szResult;
}