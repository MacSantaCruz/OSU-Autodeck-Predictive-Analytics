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
//----- CommandWhispererAddInServer.h : Declaration of the CCommandWhispererAddInServer
//-----------------------------------------------------------------------------
#pragma once

#include "resource.h"

//-----------------------------------------------------------------------------
class ATL_NO_VTABLE CCommandWhispererAddInServer : 
	public CApplicationAddInServerImpl<CCommandWhispererAddInServer, ICommandWhispererAddInServer>
{

public:
	CCommandWhispererAddInServer () {}

	DECLARE_REGISTRY_RESOURCEID(IDR_CommandWhispererAddInServer)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CCommandWhispererAddInServer)
		COM_INTERFACE_ENTRY(ICommandWhispererAddInServer)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

public:
	//- CApplicationAddInServerImpl
	STDMETHOD(OnActivate) (VARIANT_BOOL FirstTime) ;
	STDMETHOD(OnDeactivate) () ;

} ;

//-----------------------------------------------------------------------------
OBJECT_ENTRY_AUTO(__uuidof(CommandWhispererAddInServer), CCommandWhispererAddInServer)
