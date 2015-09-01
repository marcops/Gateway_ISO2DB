#ifndef _DEFINES_H_
#define _DEFINES_H_




#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#ifndef WINVER
#define WINVER 0x500
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#define LEN_SEND 5000
#define LEN_MAX_BIT 1000

#define HEADER_LEN 4
#define RECEIVE_MAX_MSG 3096



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include <list>
#include <winsock2.h>
#include <afxmt.h>
using namespace std;
#include <shlwapi.h>
#include "resource.h"
//#include <afxsock.h>            // MFC socket extensions
#define INI_FILE ".\\GatewayIn.ini"


#define _CRTDBG_MAP_ALLOC  
#define _CRTDBG_MAP_ALLOC_NEW 
//#define _INC_MALLOC
#ifdef _DEBUG
	#ifdef THIS_FILE
		#undef THIS_FILE
	#endif
#define THIS_FILE  __FILE__
	#ifndef DEBUG_NEW 
		#undef DEBUG_NEW 
		#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
	#endif
#define new DEBUG_NEW 
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#endif