// GradProgress.h: interface for the CGradProgress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADPROGRESS_H__7DA37A62_E367_11D5_AF9C_00606760170A__INCLUDED_)
#define AFX_GRADPROGRESS_H__7DA37A62_E367_11D5_AF9C_00606760170A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "MemDC.h"


/////////////////////////////////////////////////////////////////////////////
// CMemDC


class CMemDC : public CDC
{
public:
	
	// constructor sets up the memory DC
	CMemDC(CDC* pDC) : CDC()
    {
		ASSERT(pDC != NULL);
		
		m_pDC = pDC;
		m_pOldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();
		
        if (m_bMemDC)	// Create a Memory DC
		{
            pDC->GetClipBox(&m_rect);
            CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_pOldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        }
		else		// Make a copy of the relevent parts of the current DC for printing
		{
            m_bPrinting = pDC->m_bPrinting;
            m_hDC		= pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
	}
	
	// Destructor copies the contents of the mem DC to the original DC
	~CMemDC()
    {
		if (m_bMemDC) {	
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);
			
            //Swap back the original bitmap.
            SelectObject(m_pOldBitmap);
		} else {
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
            m_hDC = m_hAttribDC = NULL;
		}
	}
	
	// Allow usage as a pointer
    CMemDC* operator->() {return this;}
	
    // Allow usage as a pointer
    operator CMemDC*() {return this;}

private:
	CBitmap  m_bitmap;		// Offscreen bitmap
    CBitmap* m_pOldBitmap;	// bitmap originally found in CMemDC
    CDC*     m_pDC;			// Saves CDC passed in constructor
    CRect    m_rect;		// Rectangle of drawing area.
    BOOL     m_bMemDC;		// TRUE if CDC really is a Memory DC.
};


/******************************************************************



		GradientProcessCtrl


	- Written by matt weagle (mweagle@redrose.net)
	- Modifyed by ±èµµÇü 


 ******************************************************************/





class CGradProgress : public CProgressCtrl
{
// Construction
public:
	CGradProgress();

// Attributes
public:
// Attributes
	
	void SetRange(int nLower, int nUpper);
	int SetPos(int nPos);
	int SetStep(int nStep);
	int StepIt(void);

// Operations
public:
	
	// Set Functions
	void SetTextColor(COLORREF color)	{m_clrText = color;}
	void SetBkColor(COLORREF color)		 {m_clrBkGround = color;}
	void SetStartColor(COLORREF color)	{m_clrStart = color;}
	void SetEndColor(COLORREF color)	{m_clrEnd = color;}

	void SetText(CString strText);
	void SetHeader(CString strHeader)	{m_strHeader = strHeader;}

	// Show the percent caption
	void ShowPercent(BOOL bShowPercent = TRUE)	{m_bShowPercent = bShowPercent;}
	void ShowText(BOOL bShowText = TRUE)	{m_bShowText = bShowText;}
	
	// Get Functions
	COLORREF GetTextColor(void)	{return m_clrText;}
	COLORREF GetBkColor(void)		 {return m_clrBkGround;}
	COLORREF GetStartColor(void)	{return m_clrStart;}
	COLORREF GetEndColor(void)	{return m_clrEnd;}

	// Epoch Cnt (specialized case)
	void DisplayEpochCnt(int nCurrentCnt, int nTotalCnt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradProgress)
	//}}AFX_VIRTUAL

// Implementation
public:


	// Generated message map functions
protected:
	void DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth);	
	int m_nLower, m_nUpper, m_nStep, m_nCurrentPosition;
	COLORREF	m_clrStart, m_clrEnd, m_clrBkGround, m_clrText;
	BOOL m_bShowPercent;
	BOOL m_bShowText;
	CString m_strHeader;

	//{{AFX_MSG(CGradProgress)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GRADPROGRESS_H__7DA37A62_E367_11D5_AF9C_00606760170A__INCLUDED_)
