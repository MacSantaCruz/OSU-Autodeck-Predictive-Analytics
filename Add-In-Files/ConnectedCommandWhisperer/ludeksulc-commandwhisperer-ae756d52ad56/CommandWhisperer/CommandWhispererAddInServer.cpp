//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted,
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting documentation. 
// <YOUR COMPANY NAME> PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// <YOUR COMPANY NAME> SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE. <YOUR COMPANY NAME>, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE. 
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable
// 

//-----------------------------------------------------------------------------
//----- CommandWhispererAddInServer.cpp : Implementation of CCommandWhispererAddInServer
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "queue.h"
#include "CommandWhisperer.h"
#include "CommandWhispererControler.h"
#include "CommandWhispererAddInServer.h"

//-----------------------------------------------------------------------------
STDMETHODIMP CCommandWhispererAddInServer::OnActivate (VARIANT_BOOL FirstTime) {
	AFX_MANAGE_STATE (AfxGetStaticModuleState ()) ;
	//cirListDeque *commandSeq = createCirListDeque(); //Added code v1.1


	//- If needed, implement additional AddIn initialization code here...
	//- the m_pApplication member provides access to the Inventor Application object
	// Create the Application AddIns controller object
	CommandWhispererControler* pSampleCommand = new CommandWhispererControler(this->m_pAddInSite);
	OnErrorReturn(!pSampleCommand, E_OUTOFMEMORY);

	return S_OK ;
}

STDMETHODIMP CCommandWhispererAddInServer::OnDeactivate () {
	AFX_MANAGE_STATE (AfxGetStaticModuleState ()) ;
	//- If needed, implement additional AddIn clean-up code here...
	return S_OK ;
}


