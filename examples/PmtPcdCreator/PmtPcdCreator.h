
#if !defined(AFX_PMTPCDCREATOR_H__6A4E5CE6_EACE_4435_B083_A5FBF226DE80__INCLUDED_)
#define AFX_PMTPCDCREATOR_H__6A4E5CE6_EACE_4435_B083_A5FBF226DE80__INCLUDED_

/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Original Code is the Picture Metadata Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 1999-2000 Eastman Kodak Company. All 
 * Rights Reserved. 
 * 
 * Creation Date: 
 *
 * Original Author: 
 * Thomas L. Hosie    thomas.hosie@kodak.com
 * 
 * Contributor(s): 
*/

// PmtPcdCreator.h : main header file for the PMTPCDCREATOR application
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorApp:
// See PmtPcdCreator.cpp for the implementation of this class
//

class CPmtPcdCreatorApp : public CWinApp
{
public:
	CPmtPcdCreatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPmtPcdCreatorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPmtPcdCreatorApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMTPCDCREATOR_H__6A4E5CE6_EACE_4435_B083_A5FBF226DE80__INCLUDED_)
