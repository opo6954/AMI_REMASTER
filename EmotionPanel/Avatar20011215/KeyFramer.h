// KeyFramer.h: interface for the CKeyFramer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYFRAMER_H__DED848A5_03A1_11D2_AF36_00A0C98A1BF2__INCLUDED_)
#define AFX_KEYFRAMER_H__DED848A5_03A1_11D2_AF36_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CKeyFramer  
{
public:
	void EraseSequence();
	void RecordFrameAs(CFrameData &Frame);
	void stop();
	bool IsPlaying();
	void play();
	void FileSave(char* filename);
	void FileLoad(char* filename);
	void RecordFrame();
	void RefreshFace();
	void PreviousFrame();
	void NextFrame();
	void SetCurrentFrame(int frame);
	CKeyFramer(UINT eventid);
	CKeyFramer(UINT eventid, bool flag );
	virtual ~CKeyFramer();
	int CurrentFrame;
	int PrevFrame;

protected:
	UINT id;
	bool flagstatusbar;
	CMainFrame* pFrame;
	CAvatarView* pView;
	void CleanUpHeap();
	CFrameData* FrameData;
	CObList KeyFrameList;
	bool playing;
};

#endif // !defined(AFX_KEYFRAMER_H__DED848A5_03A1_11D2_AF36_00A0C98A1BF2__INCLUDED_)
