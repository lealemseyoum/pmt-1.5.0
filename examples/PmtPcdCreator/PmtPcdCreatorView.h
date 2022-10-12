
#if !defined(AFX_PMTPCDCREATORVIEW_H__073A3556_6F04_44DE_B119_0270833EC3B1__INCLUDED_)
#define AFX_PMTPCDCREATORVIEW_H__073A3556_6F04_44DE_B119_0270833EC3B1__INCLUDED_

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

// PmtPcdCreatorView.h : interface of the CPmtPcdCreatorView class
//
/////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PmtMetadata.h"


class ExifImageFile;
class CPictCDImageFile;

class CPmtPcdCreatorView : public CFormView
{
protected: // create from serialization only
	CPmtPcdCreatorView();
	DECLARE_DYNCREATE(CPmtPcdCreatorView)

public:
	//{{AFX_DATA(CPmtPcdCreatorView)
	enum { IDD = IDD_PMTPCDCREATOR_FORM };
	CString	m_sourceImages;
	CString	m_destDir;
	int		m_userSelectedImageSrc;
	CString	m_scannerSoftware;
	CString	m_make;
	CString	m_model;
	UINT	m_orientation;
	float	m_xRes;
	float	m_yRes;
	CString	m_dateTime;
	UINT	m_resolutionUnit;
	UINT	m_ycbcrPositioning;
	CString	m_ExifVersion;
	CString	m_dateTimeOrg;
	CString	m_dateTimeDigitized;
	CString	m_userComments;
	float	m_zoomRatio;
	UINT	m_whiteBalance;
	UINT	m_subjectRange;
	float	m_subjectDistance;
	CString	m_spatialFeqRes;
	UINT	m_sharpness;
	UINT	m_saturation;
	float	m_focalPlaneXRes;
	float	m_focalPlaneYRes;
	UINT	m_focalPlaneResUnit;
	UINT	m_pixelsXDim;
	UINT	m_pixelsYDim;
	float	m_lensFocalLength;
	CString	m_imageID;
	UINT	m_gain;
	UINT	m_focalLength35mm;
	CString	m_flashPixVer;
	UINT	m_flashfired;
	UINT	m_customRendered;
	UINT	m_constrast;
	CString	m_compConfig;
	UINT	m_colorSpace;
	UINT	m_captureType;
	float	m_brightness;
	UINT	m_ArtIlluminant;
	CString	m_subjectArea;
	UINT	m_tnCompress;
	UINT	m_tnJpegFormat;
	UINT	m_tnOrientation;
	int		m_tnResUnit;
	float	m_tnXRes;
	float	m_tnYRes;
	UINT	m_tnYcbcrPos;
	UINT	m_tnJpegFormatLength;
	CString	m_interOpIndex;
	CString	m_interOpVersion;
	CString	m_bagNumber;
	CString	m_cameraSerialNum;
	CString	m_dealerID;
	CString	m_editTagArray;
	CString	m_exposureRecord;
	BYTE	m_filmCategory;
	CString	m_filmGenCode;
	CString	m_filmProductCode;
	BYTE	m_filmSize;
	CString	m_groupCaption;
	BYTE	m_rotation;
	UINT	m_imageSource;
	float	m_magnification;
	CString	m_metadataVersion;
	float	m_navPhyXRes;
	float	m_navPhyYRes;
	int		m_navPhyResUnit;
	CString	m_orderID;
	CString	m_ownerID;
	CString	m_rollGuid;
	UINT	m_sbaColorSpace;
	CString	m_sbaShifts;
	UINT	m_scanFrameNum;
	UINT	m_printArea;
	CString	m_userAdjShifts;
	CString	m_sbaChannelDepth;
	CString	m_creationSoftware;
	long	m_imgHeight;
	long	m_imgWidth;
	CString	m_destFilenameTemplate;
	int		m_outFilenameNum;
	//}}AFX_DATA

// Attributes
public:
	CPmtPcdCreatorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPmtPcdCreatorView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPmtPcdCreatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:

	BOOL readyToGo(void);
	BOOL createPcdImageFile(void);
	void genOutputFilename(void);
	BOOL displaySrcFileData(void);
	BOOL setDestFileMetaData(void);
	BOOL checkMandatory(void);

	// Full path to selected input images
	CStringArray m_sourceImagePaths;
	
	// Holds the existing metadata of each source image file. 
	// This MD will be written to the new PCD image file.
	PmtMetadataPtr m_srcExistingMetadata;

	// Current input image path
	CString	m_inFilename;

	// Current output image path
	CString	m_outFilename;

	// If TRUE just overwrite existing files w/o asking.
	BOOL m_doNotAsk;

	// Container for current input image data
	CPictCDImageFile*	m_srcExifFile;

	// Container for current output image data
	CPictCDImageFile*	m_destExifFile;


	//{{AFX_MSG(CPmtPcdCreatorView)
	afx_msg void OnButtonBrowseSrc();
	afx_msg void OnButtonBrowseDest();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonGo();
	afx_msg void OnButtonImgId();
	afx_msg void OnButtonPrep();
	afx_msg void OnButtondDates();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PmtPcdCreatorView.cpp
inline CPmtPcdCreatorDoc* CPmtPcdCreatorView::GetDocument()
   { return (CPmtPcdCreatorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMTPCDCREATORVIEW_H__073A3556_6F04_44DE_B119_0270833EC3B1__INCLUDED_)
