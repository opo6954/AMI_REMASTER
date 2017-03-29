#if !defined(AFX_LISTCTRLEX_H__306FDA21_5DB8_11D5_AF9C_00606760170A__INCLUDED_)
#define AFX_LISTCTRLEX_H__306FDA21_5DB8_11D5_AF9C_00606760170A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:

// Operations
public:
	BOOL MoveRow(int from, int to);
	BOOL CopyRow(int from, int to);

	void MakeListHead(int nHeadCnt, char *pchHeadTitle[], int nHeadWidth[], int nFmt = LVCFMT_CENTER);

	BOOL SortTextItems(int nCol, BOOL bAscending, int low = 0 , int high = -1);
	BOOL SortNumericItems(int nCol, BOOL bAscending, int low = 0 , int high = -1);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	int nSortedCol; 
    BOOL bSortAscending; 
  

	//{{AFX_MSG(CListCtrlEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__306FDA21_5DB8_11D5_AF9C_00606760170A__INCLUDED_)
