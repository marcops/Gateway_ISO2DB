#ifndef INIREADER_H
#define INIREADER_H

#include "defines.h"
class CIniReader{
public:
	CIniReader(char* szFileName); 
	int Read(char* szSection, char* szKey, int iDefaultValue);
	double Read(char* szSection, char* szKey, float fltDefaultValue);
	bool Read(char* szSection, char* szKey, bool bolDefaultValue);
	char* Read(char* szSection, char* szKey, const char* szDefaultValue);

private:
	char m_szFileName[255];
};
#endif//INIREADER_H