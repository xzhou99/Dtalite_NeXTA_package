// CPlayerSeekBar
#pragma	once
/*
 * $Id$
 *
 * (C) 2003-2006 Gabest
 * (C) 2006-2010 see AUTHORS
 *
 * This file is part of mplayerc.
 *
 * Mplayerc is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mplayerc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
class CPlayerSeekBar : public CDialogBar
{
   DECLARE_DYNAMIC(CPlayerSeekBar)


      private:
      long m_start, m_stop, m_pos, m_posreal;
   bool m_fEnabled;

   void MoveThumb(CPoint point);
   void SetPosInternal(long pos);

   CRect GetChannelRect();
   CRect GetThumbRect();
   CRect GetInnerThumbRect();

public:
	int m_DemandLoadingStartTimeInMin;
	int m_DemandLoadingEndTimeInMin;

	int m_Player_Status;
   CPlayerSeekBar();
   long m_prev_pos;

   virtual ~CPlayerSeekBar();

   void Enable(bool fEnable);

   void GetRange(long& start, long& stop);
   void SetRange(long start, long stop);
   long GetPos(), GetPosReal();
   void SetPos(float pos);


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CPlayerSeekBar)
   virtual BOOL Create(CWnd* pParentWnd);
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG(CPlayerSeekBar)
   afx_msg void OnPaint();
   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   void ShowSimulationTime();
   afx_msg BOOL OnPlayStop(UINT nID);
};
