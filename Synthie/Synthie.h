#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"    
#include "audio/DirSound.h"	

class CSynthieApp : public CWinAppEx
{
public:
	CSynthieApp();

    CDirSound *DirSound() {return &m_DirSound;}

public:
	virtual BOOL InitInstance();

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
    CDirSound   m_DirSound;
};

extern CSynthieApp theApp;
