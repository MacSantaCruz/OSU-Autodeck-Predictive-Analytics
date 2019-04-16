/*
  DESCRIPTION

  This file contains the declaration of the class that defines the COM-object with which
  Autodesk Inventor (R) first communicates (supports the IRxApplicationAddInServer interface).

*/

#ifndef _CommandWhispererControler_
#define _CommandWhispererControler_

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class CButtonDefEvents;
class CCommandWhispererAddInServer;
class CommandWhispererDialog;
class CWUserInputEventHandler;

class CommandWhispererControler
{
	friend class CButtonDefEvents;
  private:
	CComPtr<ApplicationAddInSite> m_pApplicationSite;
	CComObject<CWUserInputEventHandler>* m_pCUserInputEventHandler;
    CButtonDefEvents* m_pButtonEvents1;
	ButtonDefinitionObjectPtr m_pBtnDef1;
	DWORD m_btnDefCookie1, m_btnDefCookie2;


	std::vector<CString> m_CommandInternalNames;//holds all InternalNames of Inventor Commands

	void SetupUI();
protected:
	CommandWhispererDialog * m_pDialog;

  public:
	  CComPtr<ApplicationAddInSite> AddinSite()
     { return m_pApplicationSite; }
	  static UINT PipeRead(LPVOID), PipeWrite(LPVOID);
	  bool quit;

  
  // Constructor(s), initializers and destructor

  public:
     CommandWhispererControler (CComPtr<ApplicationAddInSite> pAddinSite);
    ~CommandWhispererControler();
};

class CButtonDefEvents : public CCmdTarget
{
	DECLARE_DYNCREATE(CButtonDefEvents)

	CButtonDefEvents(){}; 
	CButtonDefEvents(CommandWhispererControler* pController, UINT nID);
	virtual ~CButtonDefEvents();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonDefEvents)
	//}}AFX_VIRTUAL

// Implementation
protected:

	CommandWhispererControler * m_pController;
	UINT m_nID;
	
	void OnExecuteEvent(NameValueMap* context);

	// Generated message map functions
	//{{AFX_MSG(CButtonDefEvents)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


#endif 
