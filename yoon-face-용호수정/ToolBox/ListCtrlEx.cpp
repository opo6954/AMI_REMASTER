// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{

	nSortedCol = -1; 
    bSortAscending = TRUE; 
 
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClicked) 
	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)
 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

BOOL CListCtrlEx::MoveRow(int from, int to)
{
	//Can't move to the same place, or from or to a negative index
	if(from == to || from < 0 || to < 0)
		return FALSE;

	//First Copy the row to the new location
	if(CopyRow(from, to))
	{
		//If we have just inserted a row before
		//this one in the list, we need to increment
		//our index.
		if(from > to)
			DeleteItem(from + 1);
		else
			DeleteItem(from);

		return TRUE;
	}
	else
		return FALSE;
}

BOOL CListCtrlEx::CopyRow(int from, int to)
{	
	//Can't move to the same place, or from or to a negative index
	if(from == to || from < 0 || to < 0)
		return FALSE;

	//Copy the row to the new index
	InsertItem(to, GetItemText(from, 0));

	//If row has been inserted before original
	//increment the original
	if(from > to)
		from++;

	//Loop through subitems
//	for(int i = 1; i < GetColumnCount(); i++)
	for(int i = 1; i < 3; i++)
	{
		SetItemText(to, i, GetItemText(from, i));
	}

	return TRUE;
}


/*======================================================
	리스트 컨트롤 헤더 만들기
 ======================================================*/
void CListCtrlEx::MakeListHead
(	int nHeadCnt, 
	char *pchHeadTitle[], 
	int nHeadWidth[], 
	int nFmt /* LVCFMT_LEFT */) 
{

	LV_COLUMN lvCol;

	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(int i = 0; i < nHeadCnt; i++)
	{
		lvCol.pszText = pchHeadTitle[i];
		lvCol.fmt = nFmt;
		lvCol.cx = nHeadWidth[i];
		lvCol.iSubItem = i;
		InsertColumn(i, &lvCol);
	}

	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_HEADERDRAGDROP);
}


/*----------------------------------------------------------
	Header Click 시 Sorting
 ----------------------------------------------------------*/
void CListCtrlEx::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
        HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

        if( phdn->iButton == 0 )
        {
                // User clicked on header using left mouse button
                if( phdn->iItem == nSortedCol )
                        bSortAscending = !bSortAscending;
                else
                        bSortAscending = TRUE;

                nSortedCol = phdn->iItem;
                SortTextItems( nSortedCol, bSortAscending );
				//SortNumericItems( nSortedCol, bSortAscending );

        }
        *pResult = 0;
}
  

// SortTextItems	- Sort the list based on column text
// Returns		- Returns true for success
// nCol			- column that contains the text to be sorted
// bAscending		- indicate sort order
// low			- row to start scanning from - default row is 0
// high			- row to end scan. -1 indicates last row
BOOL CListCtrlEx::SortTextItems
(	int nCol, 
	BOOL bAscending, 
	int low /* 0 */, 
	int high /* -1 */ )
{
	if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount() )
		return FALSE;

	if( high == -1 ) high = GetItemCount() - 1;

	int lo = low;
	int hi = high;
	CString midItem;

	if( hi <= lo ) return FALSE;

	midItem = GetItemText( (lo+hi)/2, nCol );

	// loop through the list until indices cross
	while( lo <= hi )
	{
		// rowText will hold all column text for one row
		CStringArray rowText;

		// find the first element that is greater than or equal to 
		// the partition element starting from the left Index.
		if( bAscending )
			while( ( lo < high ) && ( GetItemText(lo, nCol) < midItem ) )
				++lo;
		else
			while( ( lo < high ) && ( GetItemText(lo, nCol) > midItem ) )
				++lo;

		// find an element that is smaller than or equal to 
		// the partition element starting from the right Index.
		if( bAscending )
			while( ( hi > low ) && ( GetItemText(hi, nCol) > midItem ) )
				--hi;
		else
			while( ( hi > low ) && ( GetItemText(hi, nCol) < midItem ) )
				--hi;

		// if the indexes have not crossed, swap
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if( GetItemText(lo, nCol) != GetItemText(hi, nCol))
			{
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
				int nColCount = 
					((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
				rowText.SetSize( nColCount );
				int i;
				for( i=0; i<nColCount; i++)
					rowText[i] = GetItemText(lo, i);
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED | 
						LVIS_FOCUSED |  LVIS_SELECTED | 
						LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;

				lvitemhi = lvitemlo;
				lvitemhi.iItem = hi;

				GetItem( &lvitemlo );
				GetItem( &lvitemhi );

				for( i=0; i<nColCount; i++)
					SetItemText(lo, i, GetItemText(hi, i));

				lvitemhi.iItem = lo;
				SetItem( &lvitemhi );

				for( i=0; i<nColCount; i++)
					SetItemText(hi, i, rowText[i]);

				lvitemlo.iItem = hi;
				SetItem( &lvitemlo );
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortTextItems( nCol, bAscending , low, hi);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortTextItems( nCol, bAscending , lo, high );

	return TRUE;
}
  
  
/*----------------------------------------------------------
	숫자 Sorting
 ----------------------------------------------------------*/
BOOL CListCtrlEx::SortNumericItems
(	int nCol, 
	BOOL bAscending,
	int low/*=0*/,
	int high/*=-1*/
) 
{
	if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount() )
		return FALSE;
	
	if( high == -1 ) high = GetItemCount() - 1;
	int lo = low;   
	int hi = high;
	
	int midItem;
	
	if( hi <= lo ) return FALSE;
	
	midItem = atoi(GetItemText( (lo+hi)/2, nCol ));
	
	// loop through the list until indices cross
	while( lo <= hi )
	{
		// rowText will hold all column text for one row
		CStringArray rowText;
		
		// find the first element that is greater than or equal to 
		// the partition element starting from the left Index.
		if( bAscending )
			while( ( lo < high ) && (atoi(GetItemText(lo, nCol)) < midItem ) )
				++lo;           
		else
			while( ( lo < high ) && (atoi(GetItemText(lo, nCol)) > midItem ) )
				++lo;
                
		// find an element that is smaller than or equal to 
		// the partition element starting from the right Index.
		if( bAscending )
			while( ( hi > low ) && (atoi(GetItemText(hi, nCol)) > midItem ) )
				--hi;           
		else
			while( ( hi > low ) && (atoi(GetItemText(hi, nCol)) < midItem ) )
				--hi;
				
		// if the indexes have not crossed, swap                
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if(atoi(GetItemText(lo, nCol)) != atoi(GetItemText(hi, nCol)) )
			{                               
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
                
				int nColCount =
							((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
				rowText.SetSize( nColCount );
                
				int i;
				for( i=0; i < nColCount; i++)
							rowText[i] = GetItemText(lo, i);
                
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED |
							LVIS_FOCUSED |  LVIS_SELECTED |
							LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;
				lvitemhi = lvitemlo;
				lvitemhi.iItem = hi;
                
				GetItem( &lvitemlo );
				GetItem( &lvitemhi );
                
				for( i=0; i< nColCount; i++)
							SetItemText(lo, i, GetItemText(hi, i) );
                
				lvitemhi.iItem = lo;
				SetItem( &lvitemhi );
                
				for( i=0; i< nColCount; i++)
							SetItemText(hi, i, rowText[i]);
                
				lvitemlo.iItem = hi;
				SetItem( &lvitemlo );
			}
			
			++lo;
			--hi;
		}
	}
	
	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortNumericItems( nCol, bAscending , low, hi);
	
	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortNumericItems( nCol, bAscending , lo, high );
	
	return TRUE;
}
