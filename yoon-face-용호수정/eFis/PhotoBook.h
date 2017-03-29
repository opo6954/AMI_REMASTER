//{{AFX_INCLUDES()
#include "imagelist.h"
//}}AFX_INCLUDES
#if !defined(AFX_PHOTOBOOK_H__0CEA2883_A81F_4ABC_B470_A0A3583FBB49__INCLUDED_)
#define AFX_PHOTOBOOK_H__0CEA2883_A81F_4ABC_B470_A0A3583FBB49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhotoBook.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPhotoBook dialog

class CPhotoBook : public CDialog
{
// Construction
public:
	CPhotoBook(CWnd* pParent = NULL);   // standard constructor

	CStringArray *m_pResultArray;

	CDib *m_pQueryDib;
	CDib *m_pResultDib;

// Dialog Data
	//{{AFX_DATA(CPhotoBook)
	enum { IDD = IDD_PHOTOBOOK_DIALOG };
	CStatic	m_picResult;
	CStatic	m_picQuery;
	CString	m_strResultID;
	CImageList1	m_ImageList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotoBook)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPhotoBook)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTOBOOK_H__0CEA2883_A81F_4ABC_B470_A0A3583FBB49__INCLUDED_)
