// dtwDlg.h : header file
//

#if !defined(AFX_DTWDLG_H__2E281E87_D255_11D2_9A38_00105A010A6E__INCLUDED_)
#define AFX_DTWDLG_H__2E281E87_D255_11D2_9A38_00105A010A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define ORDER 12
#define MAX_WORD 200
#define OVERLAP 256
#define FR_SIZE 512


typedef struct word{
	char wordname[32]; 
	int framenum;      
	double *prCep;     
	int framenum2;     
	double *prCep2;    
}tWord;

/*
typedef struct word{
	char wordname[32];
	int framenum;
	double *prCep;
}tWord;
*/
/////////////////////////////////////////////////////////////////////////////
// CDtwDlg dialog
class CDtwDlg : public CDialog
{
// Construction
public:
	CDtwDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDtwDlg)
	enum { IDD = IDD_DTW_DIALOG };
	CListBox	m_listbox;
	CString	m_word;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDtwDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	int m_nWord;
	int m_totFr;
	tWord word[MAX_WORD];
	short* m_Data;
	//short in_Data[11025*4];
	short in_Data[11025*8];
	UINT m_FileLength;
	
	void Alert(CString msg);

	BOOL Load2Memory();
	BOOL m_openBinary();
	//void Preemphasis();
	double feat(int fr);
	void level(double* Enn, int num, double* LEL, double* UEL);
	int biginDetect(int fr, double LEL, double UEL);
	int endDetect(int begin, int fr, double LEL, double UEL);
	void m_detectEndPoint(int beginFr[], int endFr[], int *number);
	int m_trans(int beginFr[],int endFr[], int number);
	void hamming(short s[]);
	void Lpc(short *in, double *out);
	void Lpc2Cep(double *in, double *out);
	//void Normalize(double *rBuff, int nLength);
	int feat_extr1(int length);
	int feat_extr2(int length);
	void SaveAllData();
	void FreeMyMalloc();

	void OnOK();
	
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDtwDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonTrain();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DTWDLG_H__2E281E87_D255_11D2_9A38_00105A010A6E__INCLUDED_)
