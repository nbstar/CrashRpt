/************************************************************************************* 
  This file is a part of CrashRpt library.

  CrashRpt is Copyright (c) 2003, Michael Carruth
  All rights reserved.
 
  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:
 
   * Redistributions of source code must retain the above copyright notice, this 
     list of conditions and the following disclaimer.
 
   * Redistributions in binary form must reproduce the above copyright notice, 
     this list of conditions and the following disclaimer in the documentation 
     and/or other materials provided with the distribution.
 
   * Neither the name of the author nor the names of its contributors 
     may be used to endorse or promote products derived from this software without 
     specific prior written permission.
 

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************************/

// File: ResendDlg.h
// Description: 
// Authors: zexspectrum
// Date: 

#pragma once
#include <atlctrls.h>
#include <atlctrlx.h>

#define WM_RESENDTRAYICON (WM_USER+500)

class CResendDlg : 
  public CDialogImpl<CResendDlg>,   
	public CMessageFilter,
  public CDialogResize<CResendDlg>
{
public:
	enum { IDD = IDD_RESEND };
 
	virtual BOOL PreTranslateMessage(MSG* pMsg);

  BEGIN_DLGRESIZE_MAP(CResendDlg)    
    DLGRESIZE_CONTROL(IDC_TEXT, DLSZ_SIZE_X)    
    DLGRESIZE_CONTROL(IDC_LIST, DLSZ_SIZE_X|DLSZ_SIZE_Y)    
    DLGRESIZE_CONTROL(IDC_SELSIZE, DLSZ_SIZE_X|DLSZ_MOVE_Y)    
    DLGRESIZE_CONTROL(IDC_CONSENT, DLSZ_SIZE_X|DLSZ_MOVE_Y)    
    DLGRESIZE_CONTROL(IDC_PRIVACYPOLICY, DLSZ_SIZE_X|DLSZ_MOVE_Y)    
    DLGRESIZE_CONTROL(IDC_LINE, DLSZ_SIZE_X|DLSZ_MOVE_Y)    
    DLGRESIZE_CONTROL(IDC_CRASHRPT, DLSZ_MOVE_X|DLSZ_MOVE_Y)    
    DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_Y)    
    DLGRESIZE_CONTROL(IDC_OTHERACTIONS, DLSZ_MOVE_Y)    
  END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CResendDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_TIMER, OnTimer)
    MESSAGE_HANDLER(WM_RESENDTRAYICON, OnTrayIcon)    
    COMMAND_ID_HANDLER(IDOK, OnSendNow)
    COMMAND_ID_HANDLER(IDC_OTHERACTIONS, OnOtherActions)
    COMMAND_ID_HANDLER(ID_MENU3_SHOW, OnPopupShow)
    COMMAND_ID_HANDLER(ID_MENU3_EXIT, OnPopupExit)
    COMMAND_ID_HANDLER(ID_MENU4_REMINDLATER, OnRemindLater)
    COMMAND_ID_HANDLER(ID_MENU4_NEVERREMIND, OnNeverRemind)
    NOTIFY_HANDLER(IDC_LIST, LVN_ITEMCHANGED, OnListItemChanged);
    NOTIFY_HANDLER(IDC_LIST, NM_DBLCLK, OnListDblClick);
    
    CHAIN_MSG_MAP(CDialogResize<CResendDlg>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);  
  LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);	    
  LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);	    
  LRESULT OnTrayIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);	    
  LRESULT OnPopupShow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnPopupExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnListItemChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
  LRESULT OnListDblClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
  LRESULT OnSendNow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOtherActions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnRemindLater(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnNeverRemind(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  void CloseDialog(int nVal);
  void AddTrayIcon(BOOL bAdd);
  void UpdateSelectionSize();
  
  CStatic m_statText;
  CCheckListViewCtrl m_listReports;
  CSortListViewCtrl m_listReportsSort;
  CStatic m_statSize;
  CButton m_btnSendNow;
  CButton m_btnOtherActions;
  CStatic m_statConsent;
  CHyperLink m_linkPrivacyPolicy;

  int m_nTick;
};
