#if !defined(AFX_SPINPUT_H__ABA18034_2AE7_11D2_BEA9_00A0242A0A4A__INCLUDED_)
#define AFX_SPINPUT_H__ABA18034_2AE7_11D2_BEA9_00A0242A0A4A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SpInput.h : header file
//
#include "resource.h"
#include "mmsystem.h"

class Cspinput : public CDialog
{
// Construction
public:
	Cspinput(CWnd* pParent = NULL);   // standard constructor
	~Cspinput();
	PCMWAVEFORMAT WaveRecord;
	WAVEHDR WaveHeader;
	HWAVEIN hWaveInn;

	int errorCode;
	int m_nSec, m_nRecordPos;
	long lDataSize;
	long filesize;
	int m_nStatus;	//current status? idle->0 rec-> 1 play->2
	CString m_FileName;

	LPBYTE pWave;
	HANDLE waveDataBlock;

	void PlayRaw(char *filename);
	short *Loadfile(char *filename);
	int Raw2Wav(char *filename);
	int wAllocMemory();
	void wFreeMemory();
	void SetWaveInfo();
	void RecordCount();
	void wSetWaveHeader();
	int RecordWave();
	int StopRecord();
	int SaveWaveFile(LPSTR filename);
	int SaveRawFile(LPSTR filename);
	void DrawWave(CDC* pDC,CRect rect,COLORREF Color, int step, int pos);
	int Bit16Value(int num);

// Dialog Data
	//{{AFX_DATA(Cspinput)
	enum { IDD = IDD_SPEECH_INPUT };
		CSliderCtrl	m_slider;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cspinput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Cspinput)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonRecord();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonPlay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPINPUT_H__ABA18034_2AE7_11D2_BEA9_00A0242A0A4A__INCLUDED_)
