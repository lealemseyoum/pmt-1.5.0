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

/* 
PmtPcdCreatorView.cpp Description:
This example program demonstrates how to use the 
Picture Metadata Toolkit (PMT) to create Picture CD (PCD) image files.
This program is reads EXIF, JFIF and TIFF formated image files as
input. Displays a GUI that highligts the orgainzation of an PCD image
file. The GUI also highlight the Mandatory and optional PCD image file metadata. 
The GUI does NOT display all possible metadata that might be stored
in a PCD image file. The program does a minimal amount of error checking
on user entered values.

NOTE: This program uses the PmtImageAccessor. Therefore, PMT must be built
with the PMT_IMAGE_ACCESSOR_REQUIRED preprocessor definition for this
program.
 */

#include "stdafx.h"

#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// App Includes
#include "PmtPcdCreator.h"
#include "MainFrm.h"
#include "PmtPcdCreatorDoc.h"
#include "PmtPcdCreatorView.h"
#include "PictCDImageFile.h"


#undef FAR // avoid warning



// The core PMT includes ...

#include "PmtMetadataT.h"

#include "PmtDefinitionInterpreter.h"



// Each accessor that we might need has to be included so that it gets 

// set up correctly in the factory.

#include "PmtAllAccessors.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Enum for the file format of the input file selected by the user.
enum UserSelectedImageSourceEnum
{
	US_IS_NOT_DEFINED = -1,
	US_IS_DIGITAL_CAMERA,
	US_IS_JIF_FILE,
	US_IS_TIFF_FILE,
};


static const int MAX_FILENAME_LENGTH = 16384;
static const char* DEFAULT_FILENAME_TEMPLATE = "PCDC%.4d.JPG";

/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorView

IMPLEMENT_DYNCREATE(CPmtPcdCreatorView, CFormView)

BEGIN_MESSAGE_MAP(CPmtPcdCreatorView, CFormView)
	//{{AFX_MSG_MAP(CPmtPcdCreatorView)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_SRC, OnButtonBrowseSrc)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_DEST, OnButtonBrowseDest)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_GO, OnButtonGo)
	ON_BN_CLICKED(IDC_BUTTON_IMG_ID, OnButtonImgId)
	ON_BN_CLICKED(IDC_BUTTON_PREP, OnButtonPrep)
	ON_BN_CLICKED(IDC_BUTTOND_DATES, OnButtondDates)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* -------------------------------------------------------------------
	Method:	OnButtonBrowseSrc()
	Description: construction
------------------------------------------------------------------- */
CPmtPcdCreatorView::CPmtPcdCreatorView()
	: CFormView(CPmtPcdCreatorView::IDD)
{
	//{{AFX_DATA_INIT(CPmtPcdCreatorView)
	m_sourceImages = _T("");
	m_destDir = _T("");
	m_scannerSoftware = _T("");
	m_make = _T("");
	m_model = _T("");
	m_orientation = 0;
	m_xRes = 0.0f;
	m_yRes = 0.0f;
	m_dateTime = _T("");
	m_resolutionUnit = PCD_RU_UNKNOWN;
	m_ycbcrPositioning = PCD_YP_UNKNOWN;
	m_ExifVersion = _T("");
	m_dateTimeOrg = _T("");
	m_dateTimeDigitized = _T("");
	m_userComments = _T("");
	m_zoomRatio = 0.0f;
	m_whiteBalance = 0;
	m_subjectRange = 0;
	m_subjectDistance = 0.0f;
	m_spatialFeqRes = _T("");
	m_sharpness = 0;
	m_saturation = 0;
	m_focalPlaneYRes = 0.0f;
	m_focalPlaneXRes = 0.0f;
	m_focalPlaneResUnit = 0;
	m_pixelsYDim = 0;
	m_pixelsXDim = 0;
	m_lensFocalLength = 0.0f;
	m_imageID = _T("");
	m_gain = 0;
	m_focalLength35mm = 0;
	m_flashPixVer = _T("");
	m_flashfired = 0;
	m_customRendered = 0;
	m_constrast = 0;
	m_compConfig = _T("");
	m_colorSpace = 0;
	m_captureType = 0;
	m_brightness = 0.0f;
	m_ArtIlluminant = 0;
	m_subjectArea = _T("");
	m_tnCompress = 0;
	m_tnJpegFormat = 0;
	m_tnOrientation = 0;
	m_tnResUnit = 0;
	m_tnXRes = 0.0f;
	m_tnYRes = 0.0f;
	m_tnYcbcrPos = 0;
	m_tnJpegFormatLength = 0;
	m_interOpIndex = _T("");
	m_interOpVersion = _T("");
	m_bagNumber = _T("");
	m_cameraSerialNum = _T("");
	m_dealerID = _T("");
	m_editTagArray = _T("");
	m_exposureRecord = _T("");
	m_filmCategory = 0;
	m_filmGenCode = _T("");
	m_filmProductCode = _T("");
	m_filmSize = PCD_FS_UNKNOWN;
	m_groupCaption = _T("");
	m_rotation = 0;
	m_imageSource = PCD_IS_UNIDENTIFIED;
	m_magnification = 0.0f;
	m_metadataVersion = _T("");
	m_navPhyXRes = 0.0f;
	m_navPhyYRes = 0.0f;
	m_navPhyResUnit = 0;
	m_orderID = _T("");
	m_ownerID = _T("");
	m_rollGuid = _T("");
	m_sbaColorSpace = 0;
	m_sbaShifts = _T("");
	m_scanFrameNum = 0;
	m_printArea = 0;
	m_userAdjShifts = _T("");
	m_sbaChannelDepth = _T("");
	m_creationSoftware = _T("");
	m_imgHeight = 0;
	m_imgWidth = 0;
	m_destFilenameTemplate = _T(DEFAULT_FILENAME_TEMPLATE);
	m_destDir = _T("");
	m_outFilenameNum = 1;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	m_inFilename = _T("");
	m_outFilename = _T("");

	m_srcExifFile = NULL;
	m_destExifFile = NULL;
	m_doNotAsk = FALSE;

	m_userSelectedImageSrc = US_IS_NOT_DEFINED;

}

/* -------------------------------------------------------------------
	Method:	OnButtonBrowseSrc()
	Description: destruction
------------------------------------------------------------------- */
CPmtPcdCreatorView::~CPmtPcdCreatorView()
{
	if (m_srcExifFile)
	{
		delete m_srcExifFile;
		m_srcExifFile = NULL;
	}
	
	if (m_destExifFile)
	{
		delete m_destExifFile;
		m_destExifFile = NULL;
	}

	if (m_srcExistingMetadata)
	{
		m_srcExistingMetadata->deleteMetadata("*");
	}

	m_sourceImagePaths.RemoveAll();
}

void CPmtPcdCreatorView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmtPcdCreatorView)
	DDX_Text(pDX, IDC_EDIT_SOURCE_IMG, m_sourceImages);
	DDX_Text(pDX, IDC_EDIT_DEST_DIR, m_destDir);
	DDX_Text(pDX, IDC_EDIT_SCANNER_SOFTWARE, m_scannerSoftware);
	DDX_Text(pDX, IDC_EDIT_MAKE, m_make);
	DDX_Text(pDX, IDC_EDIT_MODEL, m_model);
	DDX_Text(pDX, IDC_EDIT_ORIENTATION, m_orientation);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_xRes);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_yRes);
	DDX_Text(pDX, IDC_EDIT_DATETIME, m_dateTime);
	DDX_Text(pDX, IDC_EDIT_RESOLUTION_UNIT, m_resolutionUnit);
	DDX_Text(pDX, IDC_EDIT_YCBCR_POSTIONING, m_ycbcrPositioning);
	DDX_Text(pDX, IDC_EDIT_EXIF_VERSION, m_ExifVersion);
	DDX_Text(pDX, IDC_EDIT_DAT_TIME_ORG, m_dateTimeOrg);
	DDX_Text(pDX, IDC_EDIT_DATE_TIME_DIGITIZED, m_dateTimeDigitized);
	DDX_Text(pDX, IDC_EDIT_USER_COMMENTS, m_userComments);
	DDV_MaxChars(pDX, m_userComments, 254);
	DDX_Text(pDX, IDC_EDIT_ZOOM_RATIO, m_zoomRatio);
	DDX_Text(pDX, IDC_EDIT_WHITE_BAL, m_whiteBalance);
	DDX_Text(pDX, IDC_EDIT_SUBJECT_RANGE, m_subjectRange);
	DDX_Text(pDX, IDC_EDIT_SUBJECT_DISTANCE, m_subjectDistance);
	DDX_Text(pDX, IDC_EDIT_SPATIAL_FEQ_RES, m_spatialFeqRes);
	DDX_Text(pDX, IDC_EDIT_SHARPNESS, m_sharpness);
	DDX_Text(pDX, IDC_EDIT_SATURATION, m_saturation);
	DDX_Text(pDX, IDC_EDIT_PLANE_Y_RES, m_focalPlaneYRes);
	DDX_Text(pDX, IDC_EDIT_PLANE_X_RES, m_focalPlaneXRes);
	DDX_Text(pDX, IDC_EDIT_PLANE_RES_UNIT, m_focalPlaneResUnit);
	DDX_Text(pDX, IDC_EDIT_PIXEL_Y_DIM, m_pixelsYDim);
	DDX_Text(pDX, IDC_EDIT_PIXEL_X_DIM, m_pixelsXDim);
	DDX_Text(pDX, IDC_EDIT_LENS_FOCAL, m_lensFocalLength);
	DDX_Text(pDX, IDC_EDIT_IMAGE_ID, m_imageID);
	DDX_Text(pDX, IDC_EDIT_GAIN_CONTROL, m_gain);
	DDX_Text(pDX, IDC_EDIT_FOCAL_LEN_35MM, m_focalLength35mm);
	DDX_Text(pDX, IDC_EDIT_FLASHPIX_VER, m_flashPixVer);
	DDX_Text(pDX, IDC_EDIT_FLASH_FIRED, m_flashfired);
	DDX_Text(pDX, IDC_EDIT_CUST_RENDERED, m_customRendered);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_constrast);
	DDX_Text(pDX, IDC_EDIT_COMP_CONFIG, m_compConfig);
	DDX_Text(pDX, IDC_EDIT_COLOR_SPACE, m_colorSpace);
	DDX_Text(pDX, IDC_EDIT_CAPTURE_TYPE, m_captureType);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_brightness);
	DDX_Text(pDX, IDC_EDIT_ART_ILL, m_ArtIlluminant);
	DDX_Text(pDX, IDC_EDIT_SUBJECT_AREA, m_subjectArea);
	DDX_Text(pDX, IDC_EDIT_TN_COMPRESS, m_tnCompress);
	DDX_Text(pDX, IDC_EDIT_TN_JPEG_FORMAT, m_tnJpegFormat);
	DDX_Text(pDX, IDC_EDIT_TN_ORIENTATION, m_tnOrientation);
	DDX_Text(pDX, IDC_EDIT_TN_RES_UNIT, m_tnResUnit);
	DDX_Text(pDX, IDC_EDIT_TN_XRES, m_tnXRes);
	DDX_Text(pDX, IDC_EDIT_TN_YRES, m_tnYRes);
	DDX_Text(pDX, IDC_EDIT_TN_YCBCR_POS, m_tnYcbcrPos);
	DDX_Text(pDX, IDC_EDIT_TN_JPEG_FORMAT_LENGTH, m_tnJpegFormatLength);
	DDX_Text(pDX, IDC_EDIT_INTER_OP_INDEX, m_interOpIndex);
	DDX_Text(pDX, IDC_EDIT_INTER_OP_VER, m_interOpVersion);
	DDX_Text(pDX, IDC_EDIT_BAG_NUMBER, m_bagNumber);
	DDX_Text(pDX, IDC_EDIT_CAMERA_SERIAL_NUM, m_cameraSerialNum);
	DDX_Text(pDX, IDC_EDIT_DEALER_ID, m_dealerID);
	DDX_Text(pDX, IDC_EDIT_EDIT_TAG_ARRAY, m_editTagArray);
	DDX_Text(pDX, IDC_EDIT_EXP_RECORD, m_exposureRecord);
	DDX_Text(pDX, IDC_EDIT_FILM_CATEGORY, m_filmCategory);
	DDX_Text(pDX, IDC_EDIT_FILM_GEN_CODE, m_filmGenCode);
	DDX_Text(pDX, IDC_EDIT_FILM_PROD_CODE, m_filmProductCode);
	DDX_Text(pDX, IDC_EDIT_FILM_SIZE, m_filmSize);
	DDX_Text(pDX, IDC_EDIT_GROUP_CAPTION, m_groupCaption);
	DDX_Text(pDX, IDC_EDIT_IMAGE_ROTATE, m_rotation);
	DDX_Text(pDX, IDC_EDIT_IMAGE_SRC, m_imageSource);
	DDX_Text(pDX, IDC_EDIT_MAGNIFICATION, m_magnification);
	DDX_Text(pDX, IDC_EDIT_METADAT_VERSION, m_metadataVersion);
	DDX_Text(pDX, IDC_EDIT_NAT_PHT_Y_RES, m_navPhyXRes);
	DDX_Text(pDX, IDC_EDIT_NAT_PHY_X_RES, m_navPhyYRes);
	DDX_Text(pDX, IDC_EDIT_NAT_PHY_RES_UNIT, m_navPhyResUnit);
	DDX_Text(pDX, IDC_EDIT_ORDER_ID, m_orderID);
	DDX_Text(pDX, IDC_EDIT_OWNER_ID, m_ownerID);
	DDX_Text(pDX, IDC_EDIT_ROLL_GUID, m_rollGuid);
	DDX_Text(pDX, IDC_EDIT_SBA_COLOR_SPACE, m_sbaColorSpace);
	DDX_Text(pDX, IDC_EDIT_SBA_SHIFTS, m_sbaShifts);
	DDX_Text(pDX, IDC_EDIT_SCAN_FRAME_NUM, m_scanFrameNum);
	DDX_Text(pDX, IDC_EDITPRINT_AREA, m_printArea);
	DDX_Text(pDX, IDC_EDIT_USER_ADJ_SHIFTS, m_userAdjShifts);
	DDX_Text(pDX, IDC_EDIT_SBA_CHANNEL_DEPTH, m_sbaChannelDepth);
	DDX_Text(pDX, IDC_EDIT_CREATION_SOFTWARE, m_creationSoftware);
	DDX_Text(pDX, IDC_EDIT_IMG_HEIGHT, m_imgHeight);
	DDX_Text(pDX, IDC_EDIT_IMG_WIDTH, m_imgWidth);
	DDX_Text(pDX, IDC_EDIT_DEST_FILENAME, m_destFilenameTemplate);
	DDX_Text(pDX, IDC_EDIT_FN_START_NUM, m_outFilenameNum);
	DDV_MinMaxInt(pDX, m_outFilenameNum, 0, 9999999);
	//}}AFX_DATA_MAP
}

BOOL CPmtPcdCreatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CPmtPcdCreatorView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_BROWSE_DEST);
	pWnd->EnableWindow(FALSE);

}

/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorView diagnostics

#ifdef _DEBUG
void CPmtPcdCreatorView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPmtPcdCreatorView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CPmtPcdCreatorDoc* CPmtPcdCreatorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPmtPcdCreatorDoc)));
	return (CPmtPcdCreatorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPmtPcdCreatorView message handlers


/* -------------------------------------------------------------------
	Method:	OnFileOpen()
	Description:
------------------------------------------------------------------- */
void CPmtPcdCreatorView::OnFileOpen() 
{
	OnButtonBrowseSrc();	
}

/* -------------------------------------------------------------------
	Method:	OnButtonBrowseSrc()
	Description:
------------------------------------------------------------------- */
void CPmtPcdCreatorView::OnButtonBrowseSrc() 
{

	UpdateData(TRUE);

	CFileDialog dlg(TRUE, NULL, NULL,
		OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_HIDEREADONLY,
		NULL, this);
	TCHAR szFilter[] = _T("JPEG and TIFF Files (*.jpg;*.tif)\0*.jpg;*.tif\0All Files (*.*)\0*.*\0\0");
	TCHAR szTitle[] =  _T("Select Files");
	dlg.m_ofn.lpstrFilter = szFilter;
	dlg.m_ofn.lpstrTitle = szTitle;


	// Receives list of selected filenames.
	TCHAR* fileNames;	
	fileNames = new TCHAR[MAX_FILENAME_LENGTH];

	memset(fileNames,NULL, sizeof(TCHAR) * MAX_FILENAME_LENGTH);

	// Set up for mutilple file selection.
	dlg.m_ofn.lpstrFile = (LPTSTR) fileNames;
	dlg.m_ofn.nMaxFile = MAX_FILENAME_LENGTH - 2;

	//  Display a CFileDialog to get the page template description filename
	if ( dlg.DoModal() != IDOK )
		return;

	// Clear the list of selected image file paths
	m_sourceImagePaths.RemoveAll();

	// szFileNames (dlg.m_ofn.lpstrFile) contains the directory path followed by the
	// selected filenames. The directory and filenames are NULL separated.
	// If only one file is selected then szFileNames contains the complete file path.
	
	// Get the Directory path to the selected files.
	TCHAR* dirPath = fileNames;


	// Check if only one file selected.
	if ( NULL == *(fileNames + strlen(dirPath) + 1) ) 
	{
		// Only one file, treat like a list of one.
		m_sourceImages = fileNames;

		// Set current input file now.
		m_inFilename = fileNames;

		m_sourceImagePaths.Add(m_inFilename);
	
		// Set Dest dir to image dir
		m_destDir = m_inFilename.Left(m_inFilename.ReverseFind('\\') + 1);


	}
	else
	{
		// Set Dest dir to image dir.
		m_destDir = dirPath;
		m_destDir += "\\";

		// Display directory followed by filenames
		m_sourceImages = dirPath;
		m_sourceImages += _T(";");

		TCHAR* pChar = dirPath + strlen(dirPath) + 1;

		CString srcImagePath;

		// Build list of input file paths
		while (NULL != *pChar)
		{
			// Build list of full path to image files.
			srcImagePath = m_destDir;
			srcImagePath += pChar;
			m_sourceImagePaths.Add(srcImagePath);

			m_sourceImages += pChar;

			pChar = pChar + strlen(pChar) + 1;
			m_sourceImages += _T(";");

		}

		// Get the first input file path
		m_inFilename = m_sourceImagePaths.GetAt(0);
	}

	HCURSOR hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

#if defined(HAVE_KIESEXIF) || defined(HAVE_OPENEXIF)

	// Determine image source
	ExifImageFile imgFile;

	ExifStatus status = imgFile.isExifFile((LPTSTR)(LPCTSTR)m_inFilename);
	if (EXIF_OK == status)
	{

		// Assume all exifs are from digital cameras
		m_userSelectedImageSrc = US_IS_DIGITAL_CAMERA;
		m_imageSource = PCD_IS_DIGITAL_CAMERA;

	}
	else
	{
		status = imgFile.isJpegFile((LPTSTR)(LPCTSTR)m_inFilename);
		if (EXIF_OK == status)
		{
			m_userSelectedImageSrc = US_IS_JIF_FILE;
			m_imageSource = PCD_IS_UNKNOWN_PEDIGREE;

		}
		else
		{
			// default to print scan, from all tiff files

			m_userSelectedImageSrc = US_IS_TIFF_FILE;
			m_imageSource = PCD_IS_PRINT_SCAN;

		}
	}
#else
	// default to print scan, from all tiff files
	m_userSelectedImageSrc = US_IS_TIFF_FILE;
	m_imageSource = PCD_IS_PRINT_SCAN;
#endif

	// Display this file's metadata in the GUI.
	// Any changs made by the user to this file's metadata will be merged with 
	// the the existing metadata in each selected file.
	if (!displaySrcFileData())
	{
		SetCursor(hCursor);
		MessageBox("Unable to load file.\n","Error",MB_OK);
		return;
	}


	// Now allow user to change destination dir.
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_BROWSE_DEST);
	pWnd->EnableWindow(TRUE);

	SetCursor(hCursor);
	readyToGo();
	UpdateData(FALSE);	
}

/* -------------------------------------------------------------------
	Method:	OnButtonBrowseDest()
	Description:
------------------------------------------------------------------- */
void CPmtPcdCreatorView::OnButtonBrowseDest() 
{
	CString result, temp;

	UpdateData(TRUE);

	CFileDialog dlg( FALSE, NULL, "This Directory", OFN_HIDEREADONLY, NULL, this );
	
	if ( dlg.DoModal() != IDOK )
		return;

	//
	// Now, for the fun part; parse the string so that we just have a path.
	// In other words, remove the file name, if any...
	//
	temp = dlg.GetPathName();
	result = temp.Left(dlg.m_ofn.nFileOffset);
	m_destDir = result;

	readyToGo();

	UpdateData(FALSE);
}



/* -------------------------------------------------------------------
	Method:	OnButtonExit()
	Description:
------------------------------------------------------------------- */
void CPmtPcdCreatorView::OnButtonExit() 
{
	CWinApp *pApp = NULL;
	pApp = (CWinApp*) AfxGetApp();

	pApp->m_pMainWnd->SendMessage(WM_CLOSE);

}

/* -------------------------------------------------------------------
	Method:	OnButtonGo()
	Description:
------------------------------------------------------------------- */
void CPmtPcdCreatorView::OnButtonGo() 
{
	// Get any user chages
	UpdateData(TRUE);
	
	// Is the mandatory metadata OK?
	BOOL success = checkMandatory();

	if (!success)
	{
		MessageBox("Missing some mandatory Metadata.","Verify Metadata",MB_ICONINFORMATION|MB_OK);
		return;
	}

	HCURSOR hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	// Get ready for the next iteration.
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_GO);
	pWnd->EnableWindow(FALSE);

	// For each selected fie...
	while ( success && (0 < m_sourceImagePaths.GetSize()) )
	{
		// Pop next filename off list.
		m_inFilename = m_sourceImagePaths.GetAt(0);
		m_sourceImagePaths.RemoveAt(0);

		// Delete previous file's info.
		if (m_srcExifFile)
		{
			delete m_srcExifFile;
			m_srcExifFile = NULL;
		}
		
		// Use a CPictCDImageFile object to get this file's metadata.
		m_srcExifFile = new CPictCDImageFile();
		if (!m_srcExifFile)
		{
			// error
			success = FALSE;
			break;
		}
	
		success = m_srcExifFile->load((LPCSTR) m_inFilename, m_srcExistingMetadata);
		if (!success)

		{

			continue;

		}



		// Now save this files existing metadata and the user's Gui changes
		// to a new Picture CD image file.
		success = createPcdImageFile();

	}

	// Clear list of src images on GUI.
	m_sourceImages.Empty();

	UpdateData(FALSE);	

	SetCursor(hCursor);

	if (!success)
	{
		MessageBox("Unable to create image file.","Error",MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	MessageBox("Created new file(s).","Success",MB_OK);

}
/* -------------------------------------------------------------------
	Method:	OnButtonPrep()
	Description: Make sure all the mandatory metadata is set.
------------------------------------------------------------------- */
void CPmtPcdCreatorView::OnButtonPrep() 
{
	UpdateData(TRUE);

	// Fill in App1 IFD 0
	if (m_make.GetLength() < 1)
	{
		m_make = "Eastman Kodak Company";
	}

	if (m_model.GetLength() < 1)
	{
		m_model = "PMT Picture CD Creator";
	}

	if ((m_xRes < 1) || (m_yRes < 1))
	{
		m_xRes = 72;
		m_yRes = 72;
	}

	if (m_creationSoftware.GetLength() < 1)
	{
		m_creationSoftware = "PMT Picture CD Creator.";
	}


	// Set DateTimeFileCreated to now.
	if (TRUE)
	{
		char tmpbuf[128];
		time_t ltime;
		struct tm *today;
		 
		_tzset();
		time( &ltime );

		/* Use time structure to build a customized time string. */
		today = localtime( &ltime );

		/* Use strftime to build a customized time string. */
		strftime( tmpbuf, 128,
			 "%Y:%m:%d %H:%M:%S", today );
		m_dateTime = tmpbuf;
	}

	if (PCD_RU_UNKNOWN == m_resolutionUnit)
	{
		m_resolutionUnit = PCD_RU_INCHES;
	}

	if (PCD_YP_UNKNOWN == m_ycbcrPositioning)
	{
		m_ycbcrPositioning = PCD_YP_CENTERED;
	}

	if (0 == m_orientation)
	{
		m_orientation = 1;
	}


	// App 1 IFD exif

	if ((m_pixelsXDim < 1) || (m_pixelsYDim < 1))

	{

		m_pixelsXDim = m_imgWidth;

		m_pixelsYDim = m_imgHeight;

	}
	

	m_ExifVersion = _T("0220");

	m_flashPixVer = _T("0101");

	m_compConfig.TrimLeft();
	m_compConfig.TrimRight();
	if (m_compConfig.GetLength() < 7)
	{
		m_compConfig = _T("1 2 3 0");
	}

	m_interOpIndex = _T("R98");
	
	m_interOpVersion = _T("0100");

	m_colorSpace = 1; // 1 = RGB

	// App1 IFD 1
	if (0 == m_tnCompress)
	{
		m_tnCompress = 6;
	}
	
	if (0 == m_tnOrientation)
	{
		m_tnOrientation = 1;
	}

	if (0 == m_tnResUnit)
	{
		m_tnResUnit = 2;
	}

	if (1 > m_tnXRes)
	{
		m_tnXRes = 72.0f;
	}

	if (1 > m_tnYRes)
	{
		m_tnYRes = 72.0f;
	}

	if (0 == m_tnYcbcrPos)
	{
		m_tnYcbcrPos = 1;
	}

	// App3 IFD 0
	m_metadataVersion = _T("0110");

	if (PCD_IS_UNIDENTIFIED == m_imageSource)
	{
		if (m_userSelectedImageSrc == US_IS_DIGITAL_CAMERA) 
		{
			m_imageSource = PCD_IS_DIGITAL_CAMERA;
		}
		else
		{
			if ( US_IS_JIF_FILE == m_userSelectedImageSrc)
			{
				m_imageSource = PCD_IS_UNKNOWN_PEDIGREE;
			}
			else
			{
				m_imageSource = PCD_IS_PRINT_SCAN;
			}
		}
	}

	if ((PCD_IS_FILM_SCAN == m_imageSource) && ( (m_filmSize < PCD_FS_35MM) || (PCD_FS_APS < m_filmSize) ) )
	{
		m_filmSize = PCD_FS_35MM;
	}

	if ( (PCD_IS_FILM_SCAN == m_imageSource) || (PCD_IS_PRINT_SCAN == m_imageSource) )
	{
		if (PCD_FS_APS == m_filmSize)
		{
			if (m_focalPlaneXRes <= 0.0f)
			{
				m_focalPlaneXRes = 1407.0;
			}
			if (m_focalPlaneYRes <= 0.0f)
			{
				m_focalPlaneYRes = 1407.0;
			}
			if (m_focalPlaneResUnit <= 0)
			{
				m_focalPlaneResUnit = PCD_RU_INCHES;
			}
		}
		else
		{
			if (m_focalPlaneXRes <= 0.0f)
			{
				m_focalPlaneXRes = 1131.0;
			}
			if (m_focalPlaneYRes <= 0.0f)
			{
				m_focalPlaneYRes = 1131.0;
			}
			if (m_focalPlaneResUnit <= 0)
			{
				m_focalPlaneResUnit = PCD_RU_INCHES;
			}
		}
		
		if (0 == m_scanFrameNum)
		{
			m_scanFrameNum = 1;
		}

		if (m_scannerSoftware.IsEmpty())
		{
			m_scannerSoftware = _T("Unknown");
		}
	}

	// If DateTimeDigitzed invalid set to now.
	CString temp = m_dateTimeDigitized.SpanExcluding("0123456789:");
	if ((m_dateTimeDigitized.GetLength() != 19) || (temp.GetLength() != 0) )
	{
		char tmpbuf[128];
		time_t ltime;
		struct tm *today;
		 
		_tzset();
		time( &ltime );

		/* Use time structure to build a customized time string. */
		today = localtime( &ltime );

		/* Use strftime to build a customized time string. */
		strftime( tmpbuf, 128,
			 "%Y:%m:%d %H:%M:%S", today );
		m_dateTimeDigitized = tmpbuf;
	}

	// DAteTimeOriginal - if invalid, clear it
	temp = m_dateTimeOrg.SpanExcluding("0123456789:");
	if ((19 != m_dateTimeOrg.GetLength()) || (temp.GetLength() != 0))
	{
		m_dateTimeOrg.Empty();
	}

	m_editTagArray = "1 "; //image created

	UpdateData(FALSE);	
}

/* -------------------------------------------------------------------
	Method:	OnButtonImgId()
	Description: Set image ID
------------------------------------------------------------------- */
void CPmtPcdCreatorView::OnButtonImgId() 
{	
	UpdateData(TRUE);

	GUID guid;

	if ( S_OK == CoCreateGuid(&guid) )
	{
		LPOLESTR psz;
		if ( S_OK == StringFromCLSID(guid,&psz))
		{
			m_imageID = psz;
			m_imageID.Remove('{');
			m_imageID.Remove('}');
			m_imageID.Remove('-');

			CoTaskMemFree(psz);
		}
	}

	UpdateData(FALSE);	
	
}

/* -------------------------------------------------------------------
	Method:	OnButtondDates()
	Description: Set Dates
------------------------------------------------------------------- */
void CPmtPcdCreatorView::OnButtondDates() 
{
	UpdateData(TRUE);		

	// If DateTimeDigitzed invalid set to now.
	CString temp = m_dateTimeDigitized.SpanExcluding("0123456789:");
	if ((m_dateTimeDigitized.GetLength() != 19) || (temp.GetLength() != 0) )
	{
		char tmpbuf[128];
		time_t ltime;
		struct tm *today;
		 
		_tzset();
		time( &ltime );

		/* Use time structure to build a customized time string. */
		today = localtime( &ltime );

		/* Use strftime to build a customized time string. */
		strftime( tmpbuf, 128,
			 "%Y:%m:%d %H:%M:%S", today );
		m_dateTimeDigitized = tmpbuf;
	}

	// DAteTimeOriginal - if invalid, use date time of file or m_dateTimeDigitized
	temp = m_dateTimeOrg.SpanExcluding("0123456789:");
	if ((19 != m_dateTimeOrg.GetLength()) || (temp.GetLength() != 0))
	{
	   struct _stat statBuf;
	   int fh, result;

	   fh = _open((LPCSTR) m_inFilename, _O_RDONLY);
	   if (-1 != fh)
	   {
		   result = _fstat( fh, &statBuf );

		   if (0 == result)
		   {
				struct tm *someTime;
				someTime = localtime( &(statBuf.st_ctime) );
				char tmpbuf[128];
				strftime( tmpbuf, 128,
					 "%Y:%m:%d %H:%M:%S", someTime );
				m_dateTimeOrg = tmpbuf;
		   }
		   _close(fh);
	   }
	   else
	   {
		   m_dateTimeOrg = m_dateTimeDigitized;
	   }
	}


	UpdateData(FALSE);		
}

/* -------------------------------------------------------------------
	Method:	readyToGo()
	Description: Enables the Go/Write button.
------------------------------------------------------------------- */
BOOL CPmtPcdCreatorView::readyToGo(void)
{
	if ( (US_IS_NOT_DEFINED == m_userSelectedImageSrc) || 
		m_inFilename.IsEmpty() || m_destDir.IsEmpty() )
	{
		return FALSE;
	}

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_GO);
	pWnd->EnableWindow(TRUE);

	return TRUE;
}

/* -------------------------------------------------------------------
	Method:	createPcdImageFile()
	Description:
------------------------------------------------------------------- */
BOOL CPmtPcdCreatorView::createPcdImageFile(void)
{

	genOutputFilename();

	int exist = _access((LPCSTR)m_outFilename, 0);
	if ( (!m_doNotAsk) && (-1 != exist))
	{
		CString msg;
		msg.Format("The file %s already exists.\nWhat do you want to do?\nAbort - exit, retry - over write it, Ignore - over write it and do not ask again.",(LPCSTR)m_outFilename);
		int rtn = MessageBox((LPCSTR)msg,"File Already Exists",MB_ICONQUESTION|MB_ABORTRETRYIGNORE);
		if (IDABORT == rtn)
		{
			return FALSE;
		}
		if (IDIGNORE == rtn)
		{
			m_doNotAsk = TRUE;
		}
	}

	// Use a CPictCDImageFile object to fill in Exif info frm GUI.
	if (m_destExifFile)
	{
		delete m_destExifFile;
		m_destExifFile = NULL;
	}

	m_destExifFile = new CPictCDImageFile();

	if (!m_destExifFile)
	{
		// error
		return FALSE;
	}


	// From the Gui set metadata in the destination object.
	if (!setDestFileMetaData())
	{
		// error
		return FALSE;
	}

	// Use the larger Width and Hight values.
	long width = m_srcExifFile->getPixelXDimension();
	long height = m_srcExifFile->getPixelYDimension();

	if (width < m_srcExifFile->getWidth())
	{
		width = m_srcExifFile->getWidth();
	}

	if (height < m_srcExifFile->getHeight())
	{
		height = m_srcExifFile->getHeight();
	}

	// Set in case they changed. This is likely if a TIFF file was the source.
	m_destExifFile->setPixelXDimension(width);
	m_destExifFile->setPixelYDimension(height);

	if (!m_destExifFile->setImage(m_srcExifFile->getImage()))
	{
		// error
		return FALSE;
	}
	
	// Save Metadata to the file.
	if (!(m_destExifFile->save((LPCSTR)m_outFilename, m_srcExistingMetadata)))
	{
		// error
		return FALSE;
	}
	
	return TRUE;    
}

/* -------------------------------------------------------------------
	Method:	genOutputFilename()
	Description:
------------------------------------------------------------------- */
void CPmtPcdCreatorView::genOutputFilename(void)
{
	if (m_destFilenameTemplate.GetLength() < 2)
	{
		m_destFilenameTemplate = DEFAULT_FILENAME_TEMPLATE;
	}

	CString temp;
	temp.Format((LPCSTR)m_destFilenameTemplate,m_outFilenameNum);
	m_outFilename.Format("%s%s",(LPCSTR)m_destDir,(LPCSTR)temp);

	m_outFilenameNum++;

	UpdateData(FALSE);

}


/* -------------------------------------------------------------------
	Method:	displaySrcFileData()
	Description: Reads metadata from the selected file and populates the Gui with its values.
------------------------------------------------------------------- */
BOOL CPmtPcdCreatorView::displaySrcFileData(void)
{
	if (m_srcExifFile)
	{
		delete m_srcExifFile;
		m_srcExifFile = NULL;
	}
	
	// Use a CPictCDImageFile object to get metadata info to fill in the GUI.
	m_srcExifFile = new CPictCDImageFile();
	if (!m_srcExifFile)
	{
		// error
		return FALSE;
	}

	try
	{

		if ( !(m_srcExifFile->load((LPCSTR) m_inFilename, m_srcExistingMetadata)))

		{

			throw -1; // error

		}
		// Fill in gui with values from the file
		int commentSize = 0;
		int i = 0;
		vector<unsigned char> unsignedCharVec;

		m_make = m_srcExifFile->getMake();
		m_model = m_srcExifFile->getModel();
		m_orientation = m_srcExifFile->getOrientation();
		m_xRes = m_srcExifFile->getXresolution();
		m_yRes = m_srcExifFile->getYresolution();
		m_resolutionUnit = m_srcExifFile->getResolutionUnit();
		m_creationSoftware = m_srcExifFile->getSoftware();
		m_dateTime = m_srcExifFile->getDateTime();
		m_ycbcrPositioning = m_srcExifFile->getYcbCrPositioning();
		m_imgWidth = m_srcExifFile->getWidth();
		m_imgHeight = m_srcExifFile->getHeight();
		
		unsignedCharVec.clear();
		unsignedCharVec = m_srcExifFile->getExifVersion();
		m_ExifVersion.Empty();
		for ( i = 0 ; i < unsignedCharVec.size() ; i++)
		{
			m_ExifVersion += (char) unsignedCharVec[i];
		}

		m_dateTimeOrg = m_srcExifFile->getDateTimeOriginal();
		m_dateTimeDigitized = m_srcExifFile->getDateTimeDigitized();
		m_userComments = m_srcExifFile->getUserComment();
		m_zoomRatio = m_srcExifFile->getDigitalZoomRatio();
		m_whiteBalance = m_srcExifFile->getWhiteBalance();
		m_subjectRange = m_srcExifFile->getSubjectDistanceRange();
		m_subjectDistance = m_srcExifFile->getSubjectDistance();

		vector<unsigned short> subjectAreaVec;
		subjectAreaVec = m_srcExifFile->getSubjectArea();
		m_subjectArea.Empty();
		if ( (1 < subjectAreaVec.size()) && (subjectAreaVec.size() < 5))
		{
			m_subjectArea.Empty();

			for (i = 0 ; i < subjectAreaVec.size() ; i++)
			{
				char buff[32];
				memset(buff,NULL,sizeof(buff));
				itoa(subjectAreaVec[i],buff,10);
				m_subjectArea += buff;
				m_subjectArea += " ";
			}
		}
		
		vector<unsigned char> spatialFeqResVec;
		spatialFeqResVec = m_srcExifFile->getSpatialFeqResponse();
		m_spatialFeqRes.Empty();
		for ( i = 0 ; i < spatialFeqResVec.size() ; i++)
		{
			char buff[32];
			memset(buff,NULL,sizeof(buff));
			itoa(spatialFeqResVec[i],buff,10);
			m_spatialFeqRes += buff;
			m_spatialFeqRes += " ";
		}

		m_sharpness = m_srcExifFile->getSharpness();
		m_saturation = m_srcExifFile->getSaturation();;
		m_focalPlaneXRes = m_srcExifFile->getFocalPlaneXResolution();
		m_focalPlaneYRes = m_srcExifFile->getFocalPlaneYResolution();
		m_focalPlaneResUnit = m_srcExifFile->getFocalPlaneUnitResolution();
		m_pixelsXDim = m_srcExifFile->getPixelXDimension();
		m_pixelsYDim = m_srcExifFile->getPixelYDimension();
		m_lensFocalLength = m_srcExifFile->getLensFocalLength();

		m_imageID = m_srcExifFile->getImageUniqueID();
		m_gain = m_srcExifFile->getGainControl();
		m_focalLength35mm = m_srcExifFile->getFocalLength35mm();

		unsignedCharVec.clear();
		unsignedCharVec = m_srcExifFile->getFlashPixVersion();
		m_flashPixVer.Empty();
		for ( i = 0 ; i < unsignedCharVec.size() ; i++)
		{
			m_flashPixVer += (char) unsignedCharVec[i];
		}

		
		m_flashfired = m_srcExifFile->getFlashFired();
		m_customRendered = m_srcExifFile->getCustomRendered();
		m_constrast = m_srcExifFile->getContrast();
		
		vector<unsigned char> compConfigVec;
		compConfigVec = m_srcExifFile->getComponentsConfig();
		m_compConfig.Empty();
		for ( i = 0 ; i < compConfigVec.size() ; i++)
		{
			char buff[32];
			memset(buff,NULL,sizeof(buff));
			itoa(compConfigVec[i],buff,10);
			m_compConfig += buff;
			m_compConfig += " ";
		}

		m_colorSpace = m_srcExifFile->getColorSpace();
		m_captureType = m_srcExifFile->getSceneCaptureType();
		m_brightness = m_srcExifFile->getBrightness();
		m_ArtIlluminant = m_srcExifFile->getArtificialIlluminant();

		m_interOpIndex = m_srcExifFile->getInterOpIndex();

		unsignedCharVec.clear();
		unsignedCharVec = m_srcExifFile->getInterOpVersion();
		m_interOpVersion.Empty();
		for ( i = 0 ; i < unsignedCharVec.size() ; i++)
		{
			m_interOpVersion += (char) unsignedCharVec[i];
		}


		m_tnCompress = m_srcExifFile->getTnCompression();
		m_tnJpegFormat = m_srcExifFile->getTnPEGInterchangeFormat();
		m_tnOrientation = m_srcExifFile->getTnOrientation();
		m_tnResUnit = m_srcExifFile->getTnResolutionUnit();
		m_tnXRes = m_srcExifFile->getTnXresolution();
		m_tnYRes = m_srcExifFile->getTnYresolution();
		m_tnYcbcrPos = m_srcExifFile->getTnYcbCrPositioning();
		m_tnJpegFormatLength = m_srcExifFile->getTnJPEGInterchangeFormatLength();


		// App3IFD0

		vector<unsigned char> unicodeVec;

		m_bagNumber.Empty();
		unicodeVec = m_srcExifFile->getBagNum();
		if (11 < unicodeVec.size())
		{
			for (i = 9 ; i < unicodeVec.size(); i=i+2 )
			{
				m_bagNumber += unicodeVec[i];
			}
		}
		unicodeVec.clear();

		m_cameraSerialNum.Empty();
		unicodeVec = m_srcExifFile->getCameraSerialNum();
		if (11 < unicodeVec.size())
		{
			for (int i = 9 ; i < unicodeVec.size(); i=i+2 )
			{
				m_cameraSerialNum += unicodeVec[i];
			}
		}
		unicodeVec.clear();

		m_dealerID.Empty();
		unicodeVec = m_srcExifFile->getDealerID();
		if (11 < unicodeVec.size())
		{
			for (int i = 9 ; i < unicodeVec.size(); i=i+2 )
			{
				m_dealerID += unicodeVec[i];
			}
		}
		unicodeVec.clear();

		m_ownerID.Empty();
		unicodeVec = m_srcExifFile->getCameraOwnerID();
		if (11 < unicodeVec.size())
		{
			for (int i = 9 ; i < unicodeVec.size(); i=i+2 )
			{
				m_ownerID += unicodeVec[i];
			}
		}
		unicodeVec.clear();

		m_orderID.Empty();
		unicodeVec = m_srcExifFile->getFilmOrderID();
		if (11 < unicodeVec.size())
		{
			for (int i = 9 ; i < unicodeVec.size(); i=i+2 )
			{
				m_orderID += unicodeVec[i];
			}
		}
		unicodeVec.clear();

		vector<unsigned short> uShortVec;
		char buffer[20];

		m_editTagArray.Empty();
		uShortVec = m_srcExifFile->getEditTagArray();
		for (i = 0 ; i < uShortVec.size(); i++ )
		{
			m_editTagArray += _itoa(uShortVec[i], buffer, 10);
			m_editTagArray += " ";
		}

		vector<unsigned char> uCharVec;

		m_exposureRecord.Empty();
		uCharVec = m_srcExifFile->getCompleteExposureRecord();
		for (i = 0 ; i < uCharVec.size(); i++ )
		{
			m_exposureRecord += _itoa(uCharVec[i], buffer, 10);
		}

		m_groupCaption = m_srcExifFile->getGroupCaption();

		m_filmCategory = m_srcExifFile->getFilmCategory();
		m_filmGenCode = m_srcExifFile->getFilmGenCode();
		m_filmProductCode = m_srcExifFile->getFilmProductCode();
		m_filmSize = m_srcExifFile->getFilmSize();
		m_rotation = m_srcExifFile->getImageRotationStatus();
		m_imageSource = m_srcExifFile->getImageSource();
		m_magnification = m_srcExifFile->getMagnification();
		m_scannerSoftware = m_srcExifFile->getScannerSoftware();
	
		uCharVec.clear();
		uCharVec = m_srcExifFile->getMetadataVersionNum();
		m_metadataVersion.Empty();
		for (i = 0 ; i < uCharVec.size(); i++ )
		{
			m_metadataVersion += (char) uCharVec[i];
		}
		
		m_navPhyXRes = m_srcExifFile->getNativePhysicalXResolution();
		m_navPhyYRes = m_srcExifFile->getNativePhysicalYResolution();
		m_navPhyResUnit = m_srcExifFile->getNativePhysicalResolutionUnit();

		m_rollGuid.Empty();
		uCharVec.clear();
		uCharVec = m_srcExifFile->getRollGuid();
		for (i = 0 ; i < uCharVec.size(); i++ )
		{
			m_rollGuid += uCharVec[i];
		}

		const unsigned short* uShortPtr = m_srcExifFile->getSbaInputChannelDepth();
		m_sbaChannelDepth.Format("%d %d %d",uShortPtr[0], uShortPtr[1], uShortPtr[2]);

		m_sbaColorSpace = m_srcExifFile->getSbaInputColorSpace();

		const int* uIntPtr = m_srcExifFile->getSbaRgbShifts();
		m_userAdjShifts.Format("%d %d %d",uIntPtr[0], uIntPtr[1], uIntPtr[2]);

		uIntPtr = m_srcExifFile->getUserAdjSbaRgbShifts();
		m_sbaShifts.Format("%d %d %d",uIntPtr[0], uIntPtr[1], uIntPtr[2]);

		m_scanFrameNum = m_srcExifFile->getScanFrameSeqNum();
		m_printArea = m_srcExifFile->getIntendedPrintArea();

	}
	catch(...)
	{
		return FALSE;
	}


	UpdateData(FALSE);

	return TRUE;

}

/* -------------------------------------------------------------------
	Method:	setDestFileMetaData()
	Description: Save the values from the GUI to the destination file object.
------------------------------------------------------------------- */
BOOL CPmtPcdCreatorView::setDestFileMetaData(void)
{
	if (!m_destExifFile)
	{
		return FALSE;
	}
	try
	{
		int i = 0;
		// Need to turn a string or random nubers into a vector.
		vector<unsigned short> unsignedShortVec;
		vector<unsigned char> unsignedCharVec;
		CString tmp1, tmp2;
		const char* pChar = NULL;
		int value = 0;
		int num = 0;
		char buff[16];

		m_destExifFile->setMake((LPCSTR) m_make);
		m_destExifFile->setModel((LPCSTR) m_model);
		m_destExifFile->setOrientation(m_orientation);
		m_destExifFile->setXresolution(m_xRes);
		m_destExifFile->setYresolution(m_yRes);
		m_destExifFile->setResolutionUnit(m_resolutionUnit);
		m_destExifFile->setSoftware(m_creationSoftware);
		m_destExifFile->setDateTime(m_dateTime);
		m_destExifFile->setYcbCrPositioning(m_ycbcrPositioning);

		// trim off white space
		tmp1 = m_ExifVersion;
		tmp1.TrimLeft();
		tmp1.TrimRight();

		unsignedCharVec.clear();
		
		if (4 == tmp1.GetLength())
		{
			pChar = (LPCSTR)tmp1;
			unsignedCharVec.push_back(pChar[0]);
			unsignedCharVec.push_back(pChar[1]);
			unsignedCharVec.push_back(pChar[2]);
			unsignedCharVec.push_back(pChar[3]);
		}
		m_destExifFile->setExifVersion(unsignedCharVec);

		
		m_destExifFile->setDateTimeOriginal((LPCSTR) m_dateTimeOrg);
		m_destExifFile->setDateTimeDigitized((LPCSTR) m_dateTimeDigitized);

		int numChars = m_userComments.GetLength();
		m_destExifFile->setUserComment((const unsigned char*)(LPCSTR)m_userComments, numChars);

		m_destExifFile->setDigitalZoomRatio(m_zoomRatio);
		m_destExifFile->setWhiteBalance(m_whiteBalance);
		m_destExifFile->setSubjectDistanceRange(m_subjectRange);
		m_destExifFile->setSubjectDistance(m_subjectDistance);


		// Need to turn a string or random nubers into a vector.
		// trim off white space
		tmp1 = m_subjectArea;
		tmp1.TrimLeft();
		tmp1.TrimRight();
		pChar = (LPCSTR)tmp1;
		
		num = sscanf(pChar,"%d",&value);
		unsignedShortVec.clear();

		while (1 == num)
		{
			unsignedShortVec.push_back((unsigned short) value);
			memset(buff,NULL,sizeof(buff));
			itoa(value,buff,10);
			// skip over current number
			pChar = pChar + strlen(buff);
			tmp2 = pChar;
			// trim white space
			tmp2.TrimLeft();
			tmp1 = tmp2;
			pChar = (LPCSTR)tmp1;
			num = sscanf(pChar,"%d",&value);
		}
		m_destExifFile->setSubjectArea(unsignedShortVec);


		// Need to turn a string or random nubers into a vector.
		// trim off white space
		tmp1 = m_spatialFeqRes;
		tmp1.TrimLeft();
		tmp1.TrimRight();
		pChar = (LPCSTR)tmp1;
		
		value = 0;

		num = sscanf(pChar,"%d",&value);
		unsignedCharVec.clear();

		while (1 == num)
		{
			unsignedCharVec.push_back((unsigned char) value);
			memset(buff,NULL,sizeof(buff));
			itoa(value,buff,10);
			// skip over current number
			pChar = pChar + strlen(buff);
			tmp2 = pChar;
			// trim white space
			tmp2.TrimLeft();
			tmp1 = tmp2;
			pChar = (LPCSTR)tmp1;
			num = sscanf(pChar,"%d",&value);
		}
		m_destExifFile->setSpatialFeqResponse(unsignedCharVec);

		m_destExifFile->setSharpness(m_sharpness);
		m_destExifFile->setSaturation(m_saturation);
		m_destExifFile->setFocalPlaneXResolution(m_focalPlaneXRes);
		m_destExifFile->setFocalPlaneYResolution(m_focalPlaneYRes);
		m_destExifFile->setFocalPlaneUnitResolution(m_focalPlaneResUnit);
		m_destExifFile->setPixelXDimension(m_pixelsXDim);
		m_destExifFile->setPixelYDimension(m_pixelsYDim);
		m_destExifFile->setLensFocalLength(m_lensFocalLength);

		// If user appears to want an image ID, ceate a new ID
		if (m_imageID.GetLength() > 3)
		{
		
			GUID guid;

			if ( S_OK == CoCreateGuid(&guid) )
			{
				LPOLESTR psz;
				if ( S_OK == StringFromCLSID(guid,&psz))
				{
					m_imageID = psz;
					m_imageID.Remove('{');
					m_imageID.Remove('}');
					m_imageID.Remove('-');

					CoTaskMemFree(psz);
				}
			}
		}		
		m_destExifFile->setImageUniqueID(m_imageID);

		m_destExifFile->setGainControl(m_gain);
		m_destExifFile->setFocalLength35mm(m_focalLength35mm);

		// trim off white space
		tmp1 = m_flashPixVer;
		tmp1.TrimLeft();
		tmp1.TrimRight();

		unsignedCharVec.clear();

		if (4 == tmp1.GetLength())
		{
			pChar = (LPCSTR)tmp1;
			unsignedCharVec.push_back(pChar[0]);
			unsignedCharVec.push_back(pChar[1]);
			unsignedCharVec.push_back(pChar[2]);
			unsignedCharVec.push_back(pChar[3]);
		}
		m_destExifFile->setFlashPixVersion(unsignedCharVec);
		
		m_destExifFile->setFlashFired(m_flashfired);
		m_destExifFile->setCustomRendered(m_customRendered);
		m_destExifFile->setContrast(m_constrast);
		
		// trim off white space
		tmp1 = m_compConfig;
		tmp1.TrimLeft();
		tmp1.TrimRight();
		pChar = (LPCSTR)tmp1;
		
		value = 0;

		num = sscanf(pChar,"%d",&value);
		unsignedCharVec.clear();

		while (1 == num)
		{
			unsignedCharVec.push_back((unsigned char) value);
			memset(buff,NULL,sizeof(buff));
			itoa(value,buff,10);
			// skip over current number
			pChar = pChar + strlen(buff);
			tmp2 = pChar;
			// trim white space
			tmp2.TrimLeft();
			tmp1 = tmp2;
			pChar = (LPCSTR)tmp1;
			num = sscanf(pChar,"%d",&value);
		}
		m_destExifFile->setComponentsConfig(unsignedCharVec);

		m_destExifFile->setColorSpace(m_colorSpace);
		m_destExifFile->setSceneCaptureType(m_captureType);
		m_destExifFile->setBrightness(m_brightness);
		m_destExifFile->setArtificialIlluminant(m_ArtIlluminant);

		m_destExifFile->setInterOpIndex(m_interOpIndex);

		// trim off white space
		tmp1 = m_interOpVersion;
		tmp1.TrimLeft();
		tmp1.TrimRight();
		pChar = (LPCSTR)tmp1;
		
		unsignedCharVec.clear();

		if (4 == tmp1.GetLength())
		{
			pChar = (LPCSTR)tmp1;
			unsignedCharVec.push_back(pChar[0]);
			unsignedCharVec.push_back(pChar[1]);
			unsignedCharVec.push_back(pChar[2]);
			unsignedCharVec.push_back(pChar[3]);
		}
		m_destExifFile->setInterOpVersion(unsignedCharVec);

		m_destExifFile->setTnCompression(m_tnCompress);
		m_destExifFile->setTnOrientation(m_tnOrientation);
		m_destExifFile->setTnXresolution(m_tnXRes);
		m_destExifFile->setTnYresolution(m_tnYRes);
		m_destExifFile->setTnResolutionUnit(m_tnResUnit);
		m_destExifFile->setTnJPEGInterchangeFormat(m_tnJpegFormat);
		m_destExifFile->setTnJPEGInterchangeFormatLength(m_tnJpegFormatLength);
		m_destExifFile->setTnYcbCrPositioning(m_tnYcbcrPos);

		// App3IFD0
		vector<unsigned char> unicodeVec;

		if (0 < m_bagNumber.GetLength())
		{
			unicodeVec.clear();
			unicodeVec.push_back('U');
			unicodeVec.push_back('N');
			unicodeVec.push_back('I');
			unicodeVec.push_back('C');
			unicodeVec.push_back('O');
			unicodeVec.push_back('D');
			unicodeVec.push_back('E');
			unicodeVec.push_back(NULL);
			
			const char* pChar = (LPCSTR) m_bagNumber;
			
			for (i = 0 ; i < m_bagNumber.GetLength() ; i++)
			{
				unicodeVec.push_back(NULL);
				unicodeVec.push_back(pChar[i]);
			}
			m_destExifFile->setBagNum(unicodeVec);
		}

		if (0 < m_cameraSerialNum.GetLength())
		{
			unicodeVec.clear();
			unicodeVec.push_back('U');
			unicodeVec.push_back('N');
			unicodeVec.push_back('I');
			unicodeVec.push_back('C');
			unicodeVec.push_back('O');
			unicodeVec.push_back('D');
			unicodeVec.push_back('E');
			unicodeVec.push_back(NULL);
			
			const char* pChar = (LPCSTR) m_cameraSerialNum;
			
			for (i = 0 ; i < m_cameraSerialNum.GetLength() ; i++)
			{
				unicodeVec.push_back(NULL);
				unicodeVec.push_back(pChar[i]);
			}
			m_destExifFile->setCameraSerialNum(unicodeVec);
		}

		if (0 < m_dealerID.GetLength())
		{
			unicodeVec.clear();
			unicodeVec.push_back('U');
			unicodeVec.push_back('N');
			unicodeVec.push_back('I');
			unicodeVec.push_back('C');
			unicodeVec.push_back('O');
			unicodeVec.push_back('D');
			unicodeVec.push_back('E');
			unicodeVec.push_back(NULL);
			
			const char* pChar = (LPCSTR) m_dealerID;
			
			for (i = 0 ; i < m_dealerID.GetLength() ; i++)
			{
				unicodeVec.push_back(NULL);
				unicodeVec.push_back(pChar[i]);
			}
			m_destExifFile->setDealerID(unicodeVec);
		}


		if (0 < m_ownerID.GetLength())
		{
			unicodeVec.clear();
			unicodeVec.push_back('U');
			unicodeVec.push_back('N');
			unicodeVec.push_back('I');
			unicodeVec.push_back('C');
			unicodeVec.push_back('O');
			unicodeVec.push_back('D');
			unicodeVec.push_back('E');
			unicodeVec.push_back(NULL);
			
			const char* pChar = (LPCSTR) m_ownerID;
			
			for (i = 0 ; i < m_ownerID.GetLength() ; i++)
			{
				unicodeVec.push_back(NULL);
				unicodeVec.push_back(pChar[i]);
			}
			m_destExifFile->setCameraOwnerID(unicodeVec);
		}

		m_destExifFile->setGroupCaption(m_groupCaption);

		if (0 < m_orderID.GetLength())
		{
			unicodeVec.clear();
			unicodeVec.push_back('U');
			unicodeVec.push_back('N');
			unicodeVec.push_back('I');
			unicodeVec.push_back('C');
			unicodeVec.push_back('O');
			unicodeVec.push_back('D');
			unicodeVec.push_back('E');
			unicodeVec.push_back(NULL);
			
			const char* pChar = (LPCSTR) m_orderID;
			
			for (i = 0 ; i < m_orderID.GetLength() ; i++)
			{
				unicodeVec.push_back(NULL);
				unicodeVec.push_back(pChar[i]);
			}
			m_destExifFile->setFilmOrderID(unicodeVec);
		}

		unicodeVec.clear();


		// trim off white space
		tmp1 = m_editTagArray;
		tmp1.TrimLeft();
		tmp1.TrimRight();
		pChar = (LPCSTR)tmp1;
		
		value = 0;
		vector<unsigned short> uShortVec;

		num = sscanf(pChar,"%d",&value);
		while (1 == num)
		{
			uShortVec.push_back((unsigned short) value);
			memset(buff,NULL,sizeof(buff));
			_itoa(value,buff,10);
			// skip over current number
			pChar = pChar + strlen(buff);
			tmp2 = pChar;
			// trim white space
			tmp2.TrimLeft();
			tmp1 = tmp2;
			pChar = (LPCSTR)tmp1;
			num = sscanf(pChar,"%d",&value);
		}
		m_destExifFile->setEditTagArray(uShortVec);
		
		unsignedCharVec.clear();

		pChar = (LPCSTR) m_exposureRecord;
		
		for (i = 0 ; i < m_exposureRecord.GetLength() ; i++)
		{
			unsignedCharVec.push_back(pChar[i]);
		}
		m_destExifFile->setCompleteExposureRecord(unsignedCharVec);

		m_destExifFile->setFilmCategory(m_filmCategory);
		m_destExifFile->setFilmGenCode(m_filmGenCode);
		m_destExifFile->setFilmProductCode(m_filmProductCode);
		m_destExifFile->setScannerSoftware(m_scannerSoftware);
		m_destExifFile->setFilmSize(m_filmSize);
		m_destExifFile->setImageRotationStatus(m_rotation);

		m_destExifFile->setImageSource(m_imageSource);

		m_destExifFile->setMagnification(m_magnification);
	
		tmp1 = m_metadataVersion;
		tmp1.TrimLeft();
		tmp1.TrimRight();
		
		pChar = (LPCSTR)tmp1;
		
		unsignedCharVec.clear();

		if (4 == tmp1.GetLength())
		{
			pChar = (LPCSTR)tmp1;
			unsignedCharVec.push_back(pChar[0]);
			unsignedCharVec.push_back(pChar[1]);
			unsignedCharVec.push_back(pChar[2]);
			unsignedCharVec.push_back(pChar[3]);
		}
		m_destExifFile->setMetadataVersionNum(unsignedCharVec);
		
		m_destExifFile->setNativePhysicalXResolution(m_navPhyXRes);
		m_destExifFile->setNativePhysicalYResolution(m_navPhyYRes);
		m_destExifFile->setNativePhysicalResolutionUnit(m_navPhyResUnit);

		unsignedCharVec.clear();
		for (i = 0 ; i < m_rollGuid.GetLength(); i++ )
		{
			unsignedCharVec.push_back(m_rollGuid.GetAt(i));
		}
		m_destExifFile->setRollGuid(unsignedCharVec);

		unsigned short channelDepth[3];
		sscanf((LPCSTR)m_sbaChannelDepth,"%d %d %d",&(channelDepth[0]),&(channelDepth[1]),&(channelDepth[2]) );
		m_destExifFile->setSbaInputChannelDepth(channelDepth);

		m_destExifFile->setSbaInputColorSpace(m_sbaColorSpace);

		int intArray[3];
		sscanf((LPCSTR)m_sbaShifts,"%d %d %d",&(intArray[0]),&(intArray[1]),&(intArray[2]) );
		m_destExifFile->setSbaRgbShifts(intArray);

		sscanf((LPCSTR)m_userAdjShifts,"%d %d %d",&(intArray[0]),&(intArray[1]),&(intArray[2]) );
		m_destExifFile->setUserAdjSbaRgbShifts(intArray);

		m_destExifFile->setScanFrameSeqNum(m_scanFrameNum);
		m_destExifFile->setIntendedPrintArea(m_printArea);

	}
	catch(...)
	{
		return FALSE;
	}	
	

	return TRUE;
}


/* -------------------------------------------------------------------
	Method:	checkMandatory()
	Description:
------------------------------------------------------------------- */
BOOL CPmtPcdCreatorView::checkMandatory() 
{
	UpdateData(TRUE);

	int rtn = 0;

	// Fill in App1 IFD 0
	if (m_make.GetLength() < 1)
	{
		rtn = MessageBox("Most provide a value for Make.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (m_model.GetLength() < 1)
	{
		rtn = MessageBox("Most provide a value for Model.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if ((m_xRes < 1) || (m_yRes < 1))
	{
		rtn = MessageBox("XRes and/of YRes are invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (m_creationSoftware.GetLength() < 1)
	{
		rtn = MessageBox("Most provide a value for Creation Software.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	CString temp = m_dateTime.SpanExcluding("0123456789:");

	if ((m_dateTime.GetLength() != 19) || (temp.GetLength() != 0) )
	{
		rtn = MessageBox("Date Time Created is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (PCD_RU_UNKNOWN == m_resolutionUnit)
	{
		rtn = MessageBox("Resolution Unit is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (PCD_YP_UNKNOWN == m_ycbcrPositioning)
	{
		rtn = MessageBox("ycbcrPositioning is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (0 == m_orientation)
	{
		rtn = MessageBox("Orientation is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}
	
	// App 1 IFD exif

		if ((m_pixelsXDim < 1) || (m_pixelsYDim < 1))

	{

		rtn = MessageBox("Pixel X or Y dimensions are invalid. \nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);

		if (IDOK == rtn)

		{

			return TRUE;

		}

		return FALSE;

	}


	if (m_ExifVersion.CompareNoCase("0220") != 0)
	{
		rtn = MessageBox("Exif Version is invalid. 0220\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (m_flashPixVer.CompareNoCase("0101") != 0)
	{
		rtn = MessageBox("Flash Pix Version is invalid. 0101\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	m_compConfig.TrimLeft();
	m_compConfig.TrimRight();
	if (m_compConfig.GetLength() < 7)
	{
		rtn = MessageBox("Comp Config is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (m_interOpIndex.CompareNoCase("R98") != 0)
	{
		rtn = MessageBox("Inter Op is invalid. R98\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}
	
	if (m_interOpVersion.CompareNoCase("0100") != 0)
	{
		rtn = MessageBox("Inter Op Version is invalid. 0100\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (1 != m_colorSpace)
	{
		rtn = MessageBox("Color Space is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	// App1 IFD 1
	if (0 == m_tnCompress)
	{
		rtn = MessageBox("TN Compress is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}
	
	if (0 == m_tnOrientation)
	{
		rtn = MessageBox("TN Orientation is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (0 == m_tnResUnit)
	{
		rtn = MessageBox("TN ResUnit is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (1 > m_tnXRes)
	{
		rtn = MessageBox("TN XRes is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (1 > m_tnYRes)
	{
		rtn = MessageBox("TN YRes is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if (0 == m_tnYcbcrPos)
	{
		rtn = MessageBox("TN YcbcrPos is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	// App3 IFD 0
	if (m_metadataVersion.CompareNoCase("0110") != 0)
	{
		rtn = MessageBox("Metadata Version is invalid. 0110\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}


	if ((m_imageSource < PCD_IS_UNIDENTIFIED) || (PCD_IS_UNKNOWN_PEDIGREE < m_imageSource))
	{
		rtn = MessageBox("Image Source is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}

	if ((PCD_IS_FILM_SCAN == m_imageSource) && ( (m_filmSize < PCD_FS_35MM) || (PCD_FS_APS < m_filmSize) ) )
	{
		rtn = MessageBox("Film Size is invalid.\nIgnore this and all other errors and write file?","Error",MB_ICONEXCLAMATION|MB_OKCANCEL);
		if (IDOK == rtn)
		{
			return TRUE;
		}
		return FALSE;
	}
	
	return TRUE;

}
