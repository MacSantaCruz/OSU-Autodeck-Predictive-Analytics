
/////////////////////////////////////////////////////////////////////////////
// 
// CommandWhispererDialog.cpp : implementation file
//
// This file implements the Dialog.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandWhispererControler.h"
#include "resource.h"
#include "CommandWhispererDlg.h"
#include "queue.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace std;
CommandWhispererDialog::CommandWhispererDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CommandWhispererDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CommandWhispererDialog)
	//}}AFX_DATA_INIT
}
/*
string bstr_to_str(BSTR source) {
	//source = L"lol2inside";
	_bstr_t wrapped_bstr = _bstr_t(source);
	int length = wrapped_bstr.length();
	char* char_array = new char[length];
	strcpy_s(char_array, length + 1, wrapped_bstr);
	return char_array;
}*/// added v1.1


HRESULT CommandWhispererDialog::OnActivateCommand(BSTR CommandName, NameValueMap * Context)
{
	//string commandName = bstr_to_str(CommandName); //added v1.1
	//enqueue(q, commandName);
	GetDlgItem(IDC_STATIC)->SetWindowText(CommandName);
	UNREFERENCED_PARAMETER(Context);
	return S_OK;
}

void CommandWhispererDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CommandWhispererDialog)
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CommandWhispererDialog, CDialog)
	//{{AFX_MSG_MAP(CommandWhispererDialog)
	/*ON_EN_CHANGE(IDC_OCCURENCE_CTL, OnChangeOccurence)
	ON_EN_SETFOCUS(IDC_OCCURENCE_CTL, OnSetfocusOccurence)
	ON_EN_CHANGE(IDC_OFFSETDIST_CTL, OnChangeOffset)
	ON_EN_SETFOCUS(IDC_OFFSETDIST_CTL, OnSetfocusOffset)
	ON_BN_CLICKED(IDC_WORKPLANE_BTN, OnSelectButton)*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CommandWhispererDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDOK)->EnableWindow(false);
	return TRUE;
}

void CommandWhispererDialog::OnCancel() 
{
	ShowWindow(SW_HIDE);
}

void CommandWhispererDialog::OnOK() 
{
	CDialog::OnOK();
}

BOOL CommandWhispererDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	/*if (m_pActiveView != NULL && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP))
	{
		if (pMsg->wParam == VK_F2 || pMsg->wParam == VK_F3 || pMsg->wParam == VK_F4 || pMsg->wParam == VK_F5 || pMsg->wParam == VK_F6 ||  pMsg->wParam == VK_F10)
		{
			long lViewHwnd;
			m_pActiveView->get_HWND(&lViewHwnd);

			HWND hWnd = CWnd::FromHandle((HWND)lViewHwnd)->GetWindow(GW_CHILD)->GetSafeHwnd();
			if (hWnd != NULL)
			{
				pMsg->hwnd = (HWND)hWnd;
				return false;
			}
		}
	}*/

	return CDialog::PreTranslateMessage(pMsg);
}
