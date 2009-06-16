///////////////////////////////////////////////////////////////////////////////
//
//  Module: MailMsg.cpp
//
//    Desc: See MailMsg.h
//
// Copyright (c) 2003 Michael Carruth
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MailMsg.h"
#include <atltypes.h>

CMailMsg::CMailMsg()
{
   m_lpCmcLogon      = NULL;
   m_lpCmcSend       = NULL;
   m_lpCmcLogoff     = NULL;
   m_lpMapiLogon     = NULL;
   m_lpMapiSendMail  = NULL;
   m_lpMapiLogoff    = NULL;
   m_bReady          = FALSE;
}

CMailMsg::~CMailMsg()
{
  if (m_bReady)
    MAPIFinalize();
}


void CMailMsg::SetFrom(CString sAddress)
{  
  m_from = CStringA(sAddress).GetBuffer();  
}

void CMailMsg::SetTo(CString sAddress)
{
  m_to = CStringA(sAddress).GetBuffer();
}

void CMailMsg::SetSubject(CString sSubject)
{
  m_sSubject = CStringA(sSubject).GetBuffer();   
}

void CMailMsg::SetMessage(CString sMessage) 
{
  m_sMessage = CStringA(sMessage).GetBuffer(); 
};

void CMailMsg::AddAttachment(CString sAttachment, CString sTitle)
{
  m_attachments[CStringA(sAttachment).GetBuffer()] = CStringA(sTitle).GetBuffer();  
}

BOOL CMailMsg::Initialize()
{
   m_hMapi = ::LoadLibrary(_T("mapi32.dll"));
   
   if (!m_hMapi)
      return FALSE;

   m_lpCmcQueryConfiguration = (LPCMCQUERY)::GetProcAddress(m_hMapi, "cmc_query_configuration");
   m_lpCmcLogon = (LPCMCLOGON)::GetProcAddress(m_hMapi, "cmc_logon");
   m_lpCmcSend = (LPCMCSEND)::GetProcAddress(m_hMapi, "cmc_send");
   m_lpCmcLogoff = (LPCMCLOGOFF)::GetProcAddress(m_hMapi, "cmc_logoff");
   
   m_lpMapiLogon = (LPMAPILOGON)::GetProcAddress(m_hMapi, "MAPILogon");
   m_lpMapiSendMail = (LPMAPISENDMAIL)::GetProcAddress(m_hMapi, "MAPISendMail");
   m_lpMapiLogoff = (LPMAPILOGOFF)::GetProcAddress(m_hMapi, "MAPILogoff");

   m_bReady = (m_lpCmcLogon && m_lpCmcSend && m_lpCmcLogoff) ||
              (m_lpMapiLogon && m_lpMapiSendMail && m_lpMapiLogoff);

   return m_bReady;
}

void CMailMsg::Finalize()
{
   ::FreeLibrary(m_hMapi);
}

BOOL CMailMsg::Send()
{
  if(MAPISend())
    return TRUE;

  if(CMCSend())
    return TRUE;

  return FALSE;
}

BOOL CMailMsg::MAPISend()
{
   TStrStrMap::iterator p;
   int                  nIndex = 0;
   int                  nRecipients = 0;
   MapiRecipDesc*       pRecipients = NULL;
   int                  nAttachments = 0;
   MapiFileDesc*        pAttachments = NULL;
   ULONG                status = 0;
   MapiMessage          message;

   if(!m_bReady && !Initialize())
     return FALSE;
   
   pRecipients = new MapiRecipDesc[2];
   if(!pRecipients)
     return FALSE;

   nAttachments = (int)m_attachments.size();
   if (nAttachments)
     pAttachments = new MapiFileDesc[nAttachments];
   if(!pAttachments)
     return FALSE;

   // set from
   pRecipients[nIndex].ulReserved = 0;
   pRecipients[nIndex].ulRecipClass = MAPI_ORIG;
   pRecipients[nIndex].lpszAddress = m_from;
   pRecipients[nIndex].lpszName = "";
   pRecipients[nIndex].ulEIDSize = 0;
   pRecipients[nIndex].lpEntryID = NULL;
   nIndex++;
   
   // set to
   pRecipients[nIndex].ulReserved = 0;
   pRecipients[nIndex].ulRecipClass = MAPI_TO;
   pRecipients[nIndex].lpszAddress = m_to.begin()->first;
   pRecipients[nIndex].lpszName = m_to.begin()->second;
   pRecipients[nIndex].ulEIDSize = 0;
   pRecipients[nIndex].lpEntryID = NULL;
   nIndex++;
   
   // add attachments
    for (p = m_attachments.begin(), nIndex = 0;
      p != m_attachments.end(); p++, nIndex++)
    {
      pAttachments[nIndex].ulReserved        = 0;
      pAttachments[nIndex].flFlags           = 0;
      pAttachments[nIndex].nPosition         = 0xFFFFFFFF;
      pAttachments[nIndex].lpszPathName      = p->first;
      pAttachments[nIndex].lpszFileName      = p->second;
      pAttachments[nIndex].lpFileType        = NULL;
    }
    
    message.ulReserved                        = 0;
    message.lpszSubject                       = (LPTSTR)m_sSubject.GetBuffer();
    message.lpszNoteText                      = (LPTSTR)m_sMessage.GetBuffer();
    message.lpszMessageType                   = NULL;
    message.lpszDateReceived                  = NULL;
    message.lpszConversationID                = NULL;
    message.flFlags                           = 0;
    message.lpOriginator                      = m_from.size() ? pRecipients : NULL;
    message.nRecipCount                       = (ULONG)nRecipients - m_from.size(); // don't count originator
    message.lpRecips                          = nRecipients - m_from.size() ? &pRecipients[m_from.size()] : NULL;
    message.nFileCount                        = nAttachments;
    message.lpFiles                           = nAttachments ? pAttachments : NULL;

    status = m_lpMapiSendMail(0, 0, &message, MAPI_DIALOG, 0);

    if (pRecipients)
       delete [] pRecipients;

    if (nAttachments)
         delete [] pAttachments;
   
   return (SUCCESS_SUCCESS == status);
}

BOOL CMailMsg::CMCSend()
{
  TStrStrMap::iterator p;
  int                  nIndex = 0;
  CMC_recipient*       pRecipients;
  CMC_attachment*      pAttachments;
  CMC_session_id       session;
  CMC_return_code      status = 0;
  CMC_message          message;
  CMC_boolean          bAvailable = FALSE;
  CMC_time             t_now = {0};

  if (!m_bReady && !Initialize())
    return FALSE;

  pRecipients = new CMC_recipient[2];
  pAttachments = new CMC_attachment[m_attachments.size()];

  // set to
  pRecipients[nIndex].name = m_to.begin()->second.GetBuffer();
  pRecipients[nIndex].name_type = CMC_TYPE_INDIVIDUAL;
  pRecipients[nIndex].address = (CMC_string)(LPCSTR)m_to;
  pRecipients[nIndex].role = CMC_ROLE_TO;
  pRecipients[nIndex].recip_flags = 0;
  pRecipients[nIndex].recip_extensions = NULL;
   
  // set from
  pRecipients[nIndex+1].name = m_from.begin()->second.GetBuffer();
  pRecipients[nIndex+1].name_type = CMC_TYPE_INDIVIDUAL;
  pRecipients[nIndex+1].address = (CMC_string)(LPCSTR)m_from;
  pRecipients[nIndex+1].role = CMC_ROLE_ORIGINATOR;
  pRecipients[nIndex+1].recip_flags = CMC_RECIP_LAST_ELEMENT;
  pRecipients[nIndex+1].recip_extensions = NULL;
   
  // add attachments
  for (p = m_attachments.begin(), nIndex = 0;
    p != m_attachments.end(); p++, nIndex++)
  {
    pAttachments[nIndex].attach_title       = p->second.GetBuffer();
    pAttachments[nIndex].attach_type        = NULL;
    pAttachments[nIndex].attach_filename    = (CMC_string)(LPCSTR)p->first;
    pAttachments[nIndex].attach_flags       = 0;
    pAttachments[nIndex].attach_extensions  = NULL;
  }
  
  pAttachments[nIndex-1].attach_flags        = CMC_ATT_LAST_ELEMENT;

  message.message_reference                 = NULL;
  message.message_type                      = NULL;
  CStringA sSubject = CStringA(m_sSubject);
  CStringA sMessage = CStringA(m_sMessage);
  message.subject                           = sSubject.GetBuffer();
  message.time_sent                         = t_now;
  message.text_note                         = sMessage.GetBuffer();
  message.recipients                        = pRecipients;
  message.attachments                       = pAttachments;
  message.message_flags                     = 0;
  message.message_extensions                = NULL;

  status = m_lpCmcQueryConfiguration(
    0, 
    CMC_CONFIG_UI_AVAIL, 
    (void*)&bAvailable, 
    NULL
    );

  if (CMC_SUCCESS == status && bAvailable)
  {
    status = m_lpCmcLogon(
      NULL,
      NULL,
      NULL,
      NULL,
      0,
      CMC_VERSION,
      CMC_LOGON_UI_ALLOWED |
      CMC_ERROR_UI_ALLOWED,
      &session,
      NULL
      );

    if (CMC_SUCCESS == status)
    {
      status = m_lpCmcSend(session, &message, 0, 0, NULL);
      m_lpCmcLogoff(session, NULL, CMC_LOGON_UI_ALLOWED, NULL);
    }
  }

  delete [] pRecipients;
  delete [] pAttachments;

  return ((CMC_SUCCESS == status) && bAvailable);
}
