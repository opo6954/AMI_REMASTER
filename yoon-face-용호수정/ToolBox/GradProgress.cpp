// GradProgress.cpp: implementation of the CGradProgress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GradProgress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



/******************************************************************



		GradientProcessCtrl


	- Written by matt weagle (mweagle@redrose.net)
	- Modifyed by 김도형 


 ******************************************************************/


/////////////////////////////////////////////////////////////////////////////
// CGradProgress

CGradProgress::CGradProgress()
{
	// Defaults assigned by CProgressCtrl()
	m_nLower = 0;
	m_nUpper = 100;
	m_nCurrentPosition = 0;
	m_nStep = 10;	
	
	// Initial colors
	m_clrStart	= COLORREF(RGB(100, 200, 200));
	m_clrEnd =	 COLORREF(RGB(0,0,0));
	m_clrBkGround = ::GetSysColor(COLOR_3DFACE);
	m_clrText = COLORREF(RGB(0, 255, 0));

	// Initial show percent
	m_bShowPercent = FALSE;
	m_bShowText = TRUE;

	m_strHeader = "";
}



BEGIN_MESSAGE_MAP(CGradProgress, CProgressCtrl)
	//{{AFX_MSG_MAP(CGradProgress)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradProgress message handlers

/************************************************************************/
// The main drawing routine.  Consists of two parts
// (1) Call the DrawGradient routine to draw the visible part of the progress gradient
// (2) If needed, show the percentage text
/************************************************************************/
void CGradProgress::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// If the current positionis  invalid then we should fade into the  background
	if (m_nCurrentPosition <= m_nLower || m_nCurrentPosition > m_nUpper)
	{
		CRect rect;
		GetClientRect(rect);
		CBrush brush;
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
		dc.FillRect(&rect, &brush);
		VERIFY(brush.DeleteObject());
		return;
	}


	// Figure out what part should be visible so we can stop the gradient when needed
	CRect rectClient;
	GetClientRect(&rectClient);
	float maxWidth((float)m_nCurrentPosition/(float)m_nUpper * (float)rectClient.right);
	


	// Draw the gradient
	DrawGradient(&dc, rectClient, (int)maxWidth);

	// Show percent indicator if needed
	if (m_bShowPercent || m_bShowText)
	{
		CString strTemp;
		if(m_bShowPercent)
			strTemp.Format("%s%.0f%%", m_strHeader, 100.0f*(float)m_nCurrentPosition/(float)m_nUpper);
		else if(m_bShowText)
			strTemp.Format("%s%d/%d", m_strHeader, m_nCurrentPosition, m_nUpper);
		dc.SetTextColor(m_clrText);
		dc.SetBkMode(TRANSPARENT);

		rectClient.right = (int)maxWidth;
		dc.DrawText(strTemp, &rectClient, DT_VCENTER |  DT_CENTER | DT_SINGLELINE);
		
	}



	// Do not call CProgressCtrl::OnPaint() for painting messages
}


/*************************************************************************/
// Need to keep track of wher the indicator thinks it is.
/*************************************************************************/
void CGradProgress:: SetRange(int nLower, int nUpper)
{
	m_nLower = nLower;
	m_nUpper = nUpper;
	m_nCurrentPosition = nLower;
	CProgressCtrl::SetRange(nLower, nUpper);
}

/*************************************************************************/
// Need to keep track of wher the indicator thinks it is.
/*************************************************************************/
int CGradProgress:: SetPos(int nPos)
{
	m_nCurrentPosition = nPos;
	return (CProgressCtrl::SetPos(nPos));
}

/*************************************************************************/
// Need to keep track of wher the indicator thinks it is.
/*************************************************************************/
int CGradProgress:: SetStep(int nStep)
{
	m_nStep = nStep;
	return (CProgressCtrl::SetStep(nStep));
}

/*************************************************************************/
// Need to keep track of wher the indicator thinks it is.
/*************************************************************************/
int CGradProgress:: StepIt(void)
{
	m_nCurrentPosition += m_nStep;
	return (CProgressCtrl::StepIt());
}

/*************************************************************************/
// Where most of the actual work is done.  The general version would fill the entire rectangle with
// a gradient, but we want to truncate the drawing to reflect the actual progress control position.
/*************************************************************************/
void CGradProgress::DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth)
{
	RECT rectFill;			   // Rectangle for filling band
	float fStep;              // How wide is each band?
	CBrush brush;			// Brush to fill in the bar	

	
	CMemDC memDC(pDC);

	// First find out the largest color distance between the start and end colors.  This distance
	// will determine how many steps we use to carve up the client region and the size of each
	// gradient rect.
	int r, g, b;							// First distance, then starting value
	float rStep, gStep, bStep;		// Step size for each color

	// Get the color differences
	r = (GetRValue(m_clrEnd) - GetRValue(m_clrStart));
	g = (GetGValue(m_clrEnd) - GetGValue(m_clrStart));
	b =  (GetBValue(m_clrEnd) - GetBValue(m_clrStart));


	// Make the number of steps equal to the greatest distance
	int nSteps = max(abs(r), max(abs(g), abs(b)));

	// Determine how large each band should be in order to cover the
	// client with nSteps bands (one for every color intensity level)
	fStep = (float)rectClient.right / (float)nSteps;

	// Calculate the step size for each color
	rStep = r/(float)nSteps;
	gStep = g/(float)nSteps;
	bStep = b/(float)nSteps;

	// Reset the colors to the starting position
	r = GetRValue(m_clrStart);
	g = GetGValue(m_clrStart);
	b = GetBValue(m_clrStart);


	// Start filling bands
	for (int iOnBand = 0; iOnBand < nSteps; iOnBand++) 
	{
		
		::SetRect(&rectFill,
					(int)(iOnBand * fStep),       // Upper left X
					 0,									 // Upper left Y
					(int)((iOnBand+1) * fStep),          // Lower right X
					rectClient.bottom+1);			// Lower right Y
	
		// CDC::FillSolidRect is faster, but it does not handle 8-bit color depth
		VERIFY(brush.CreateSolidBrush(RGB(r+rStep*iOnBand, g + gStep*iOnBand, b + bStep *iOnBand)));
		memDC.FillRect(&rectFill,&brush);
		VERIFY(brush.DeleteObject());


		// If we are past the maximum for the current position we need to get out of the loop.
		// Before we leave, we repaint the remainder of the client area with the background color.
		if (rectFill.right > nMaxWidth)
		{
			::SetRect(&rectFill, rectFill.right, 0, rectClient.right, rectClient.bottom);
			VERIFY(brush.CreateSolidBrush(m_clrBkGround));
			memDC.FillRect(&rectFill, &brush);
			VERIFY(brush.DeleteObject());
			return;
		}
	}
}

/*************************************************************************/
// All drawing is done in the OnPaint function
/*************************************************************************/
BOOL CGradProgress::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;

}


/*==============================================================
	프로그래스바가 동작하기 전 텍스트를 삽입할 필요가 있을 경우
 ==============================================================*/
void CGradProgress::SetText(CString strText) 
{

	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));

	CDC *pDC = GetDC();
	CRect rectClient;
	GetClientRect(rectClient);
	pDC->FillRect(&rectClient, &brush);		// Clear Background

	// Draw Text
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strText, &rectClient, DT_VCENTER |  DT_CENTER | DT_SINGLELINE);

	brush.DeleteObject();
}


/*----------------------------------------------------------
	현재의 반복회수를 프로그래스바 로 표시
 ----------------------------------------------------------*/
void CGradProgress::DisplayEpochCnt(int nCurrentCnt, int nTotalCnt)
{

	if(nCurrentCnt == 0)	 // ProgressBar Setting
	{
		SetRange(0, nTotalCnt);
		ShowText(TRUE);
	}

	SetPos(nCurrentCnt+1);
	//PeekAndPump();			// Message Handle
}
