/*
  DESCRIPTION

  This file contains the implementation of the class that offers the first contact with
  Inventor.

*/

#include "stdafx.h"

#include "CommandWhispererControler.h"
#include "CommandWhisperer.h"
#include "CommandWhispererDlg.h"
#include "UserInputEventHandler.h"
#include "AfxCtl.h"


// The addin wizard adds command id definitions here.
#define SHOW_DLG			1


/*---------------------- Constructor(s), initializers and destructor ---------------------------------*/

CommandWhispererControler::CommandWhispererControler (CComPtr<ApplicationAddInSite> pAddinSite):m_pCUserInputEventHandler(NULL)
{ 
	m_pApplicationSite = pAddinSite;

	m_pButtonEvents1 = new CButtonDefEvents(this, SHOW_DLG);

	m_btnDefCookie1 = 0;
	SetupUI();
}

CommandWhispererControler::~CommandWhispererControler()
{
	AFX_MANAGE_STATE (AfxGetAppModuleState()); 

	if(m_pButtonEvents1)
		delete m_pButtonEvents1;

	m_pBtnDef1 = NULL;
	IUtilUserInputEventHandler::Disconnect(&m_pCUserInputEventHandler);
	if( m_pDialog)
		delete m_pDialog;
}

void CommandWhispererControler::SetupUI()
{
	ATLASSERT(m_pApplicationSite);

	if(!m_pApplicationSite)
		return;

	CComPtr<CommandManager> pCommandMgr;
	HRESULT hr = m_pApplicationSite->GetApplication()->get_CommandManager(&pCommandMgr);

	CComPtr<ControlDefinitions> pCtrlDefs;
	hr = pCommandMgr->get_ControlDefinitions(&pCtrlDefs);

	//example of getting all the commands
	long iCtrlDefsCount = pCtrlDefs->Count;
	for (long i = 1; i < iCtrlDefsCount+1; ++i) {
		ControlDefinitionPtr pCtrlDef = pCtrlDefs->GetItem(i);
		m_CommandInternalNames.push_back((LPCTSTR)pCtrlDef->InternalName);
	}

	// Create the Dialog
	m_pDialog = new CommandWhispererDialog();
	LONG_PTR lMainFrameHWND = m_pApplicationSite->GetApplication()->MainFrameHWND;
	m_pDialog->Create(IDD_MAIN_WINDOW, CWnd::FromHandle((HWND)lMainFrameHWND));

	//Connect User Input Events
	hr = IUtilUserInputEventHandler::Connect(m_pApplicationSite->GetApplication(), m_pDialog, &m_pCUserInputEventHandler);
	//ASSERT_VALID(m_pDialog);
	
	CComVariant vtAddInId;
	CComBSTR InternalNameBSTR;
	CComBSTR displayNameBSTR;
	CComBSTR DesTextBSTR;
	CComBSTR TooltipTextBSTR;
	CComVariant vtEmpty;
	CommandTypesEnum eCmdType;
	ButtonDisplayEnum eCmdDisplayType;

	// common button parameters
	// client Id for the buttons
	vtAddInId	      = _T("{78FA6571-1D45-41E2-92E8-F80A1E4CD4A1}");
	// command type
	eCmdType = kQueryOnlyCmdType;
	// button display type
	eCmdDisplayType = kAlwaysDisplayText;

	// Add the Replicate Workplane command
	InternalNameBSTR  =	_T("CommandWhisperer.Dialog");
	displayNameBSTR	  =	_T("CommandWhisperer");
	DesTextBSTR		  =	_T("Display CommandWhisperer Dialog");
	TooltipTextBSTR	  =	_T("CommandWhisperer Dialog");	

	hr = pCtrlDefs->AddButtonDefinition(displayNameBSTR, InternalNameBSTR,eCmdType,vtAddInId,DesTextBSTR,TooltipTextBSTR,vtEmpty,vtEmpty,eCmdDisplayType,&m_pBtnDef1);

	ATLASSERT(SUCCEEDED(hr));
	ATLASSERT(m_pBtnDef1 != NULL);

	m_pBtnDef1->put_Enabled(TRUE);

	if(m_pBtnDef1)
	{
		BOOL bBrwsrAdvised = AfxConnectionAdvise(m_pBtnDef1, DIID_ButtonDefinitionSink,
                                m_pButtonEvents1->GetInterface(&IID_IUnknown),
                                TRUE, &m_btnDefCookie1);

		ATLASSERT(bBrwsrAdvised == TRUE);
	}

	hr = m_pBtnDef1->AutoAddToGUI();
	ATLASSERT(SUCCEEDED(hr));
}


/////////////////////////////////////////////////////////////////////////////
// CButtonDefHandlerEvents

IMPLEMENT_DYNCREATE(CButtonDefEvents, CCmdTarget)

CButtonDefEvents::CButtonDefEvents(CommandWhispererControler* pController, UINT nID) : m_pController(pController), m_nID(nID)
{
   EnableAutomation();  // Needed in order to sink events.
}

CButtonDefEvents::~CButtonDefEvents()
{
}


BEGIN_MESSAGE_MAP(CButtonDefEvents, CCmdTarget)
	//{{AFX_MSG_MAP(CButtonDefEvents)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CButtonDefEvents, CCmdTarget)
	DISP_FUNCTION_ID(CButtonDefEvents, "OnExecute", 0x03009c81, OnExecuteEvent, VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CButtonDefEvents, CCmdTarget)
	INTERFACE_PART(CButtonDefEvents, DIID_ButtonDefinitionSink, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonDefHandlerEvents event handlers

void CButtonDefEvents::OnExecuteEvent(NameValueMap* context) 
{
	//show dialog here
	m_pController->m_pDialog->ShowWindow(SW_SHOW);
}
