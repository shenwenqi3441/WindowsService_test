// MFCLibrary1.h: MFCLibrary1 DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号


// CMFCLibrary1App
// 有关此类实现的信息，请参阅 MFCLibrary1.cpp
//

class CMFCLibrary1App : public CWinApp
{
public:
	CMFCLibrary1App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
