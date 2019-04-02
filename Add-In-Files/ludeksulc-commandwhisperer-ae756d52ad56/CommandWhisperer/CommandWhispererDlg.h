#ifndef _CommandWhispererDlg_
#define _CommandWhispererDlg_
#include <string>

/////////////////////////////////////////////////////////////////////////////
// CommandWhispererDialog
//
// This defines the code needed for the dialog.
//
/////////////////////////////////////////////////////////////////////////////
#include "UserInputEventHandler.h"
using namespace std;
class CommandWhispererDialog : public CDialog, public IUtilUserInputEventHandler
{
// Construction
public:
	CommandWhispererDialog(CWnd* pParent = NULL);   // standard constructor
	//string bstr_to_str(BSTR source); //Added code v1.1
   //virtual HRESULT OnActivateCommand(BSTR CommandName, NameValueMap * Context, struct cirListDeque *q);  //Added code v1.1
	HRESULT OnActivateCommand(BSTR CommandName, NameValueMap * Context);
// Dialog Data
	//{{AFX_DATA(ApiNonModalDialog)
	enum {
		IDD = IDR_CommandWhisperer
	};
	CButton	m_OkButton;
	CButton	m_CancelButton;
	//}}AFX_DATA

	BOOL OnInitDialog();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CommandWhispererDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CommandWhispererDialog)
	virtual void OnCancel();
	virtual void OnOK();

	USE_INV_REGDLL_HANDLING(CRegHandling)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CComPtr<View> m_pActiveView;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 