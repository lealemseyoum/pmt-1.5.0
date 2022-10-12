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

// PmtPcdCreatorDoc.cpp : implementation of the CPmtPcdCreatorDoc class
//

#include "stdafx.h"
#include "PmtPcdCreator.h"

#include "PmtPcdCreatorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorDoc

IMPLEMENT_DYNCREATE(CPmtPcdCreatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CPmtPcdCreatorDoc, CDocument)
	//{{AFX_MSG_MAP(CPmtPcdCreatorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorDoc construction/destruction

CPmtPcdCreatorDoc::CPmtPcdCreatorDoc()
{
	// TODO: add one-time construction code here

}

CPmtPcdCreatorDoc::~CPmtPcdCreatorDoc()
{
}

BOOL CPmtPcdCreatorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorDoc serialization

void CPmtPcdCreatorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorDoc diagnostics

#ifdef _DEBUG
void CPmtPcdCreatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPmtPcdCreatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorDoc commands
