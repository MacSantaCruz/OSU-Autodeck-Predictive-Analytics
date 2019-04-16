//CODE_COVERAGE_FEATURE(ADDIN FRAMEWORK)


#include "stdafx.h"
#include "UserInputEventHandler.h"


/////////////////////////////////////////////////////////////////////////////
// CUserInputEventHandler

STDMETHODIMP CWUserInputEventHandler::OnStartCommand(CommandIDEnum CommandID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try{
		if (m_pParent)
			return m_pParent->OnStartCommand(CommandID);
	}catch (_com_error e){
		ATLASSERT(0);
		return e.Error();
	}
	return S_OK;

}
	
STDMETHODIMP CWUserInputEventHandler::OnStopCommand( CommandIDEnum CommandID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try{
		if (m_pParent)
			return m_pParent->OnStopCommand(CommandID);
	}catch (_com_error e){
		ATLASSERT(0);
		return e.Error();
	}
	
	return S_OK;

}
	
//This method isn't specifically meant to handle Adds only, it's actually our implementation on OnContextMenuOld
// but since it doesn't delete several item types and the other method isn't safe in the creation
// Workflow we're using this method to add and the other to delete
// items from the context menu as it works for all entities while ONContextMenuOld will not delete
// Flyouts and other types.
STDMETHODIMP CWUserInputEventHandler::OnContextMenuOld(SelectionDeviceEnum SelectionDevice,  
							NameValueMap * AdditionalInfo, CommandBar * CommandBar)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try{
		if (m_pParent)
			return m_pParent->OnContextMenuOld(SelectionDevice,AdditionalInfo,CommandBar );
	}catch (_com_error e){
		ATLASSERT(0);
		return e.Error();
	}
	
	return S_OK;
}
//This method isn't specifically meant to handle Deletes only, it's actually our implementation on OnContext Menu
// but since it's fnot know to be safe in the Creation workflow we're just using it to delete
// items from the context menu as it works for all entities while ONContextMenuOld will not delete
// Flyouts and other types.
STDMETHODIMP CWUserInputEventHandler::OnContextMenuDelete(SelectionDeviceEnum SelectionDevice,  
							NameValueMap * AdditionalInfo, CommandBar * CommandBar)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try{
		if (m_pParent)
			return m_pParent->OnContextMenuDelete(SelectionDevice,AdditionalInfo,CommandBar );
	}catch (_com_error e){
		ATLASSERT(0);
		return e.Error();
	}
	
	return S_OK;
}


STDMETHODIMP CWUserInputEventHandler::OnDrag(DragStateEnum DragState, ShiftStateEnum ShiftKeys, 
									Point * ModelPosition, Point2d * ViewPosition, View * View, 
									NameValueMap * AdditionalInfo, HandlingCodeEnum * HandlingCode )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try{
		if (m_pParent)
			return m_pParent->OnDrag(DragState, ShiftKeys, ModelPosition, ViewPosition, View, 
									 AdditionalInfo, HandlingCode);
	}catch (_com_error e){
		ATLASSERT(0);
		return e.Error();
	}
	
	return S_OK;
}


STDMETHODIMP CWUserInputEventHandler::OnActivateCommand(BSTR CommandName, NameValueMap * Context)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try{
		if (m_pParent)
			return m_pParent->OnActivateCommand(CommandName, Context);
	}catch (_com_error e){
		ATLASSERT(0);
		return e.Error();
	}

	return S_OK;
}


STDMETHODIMP CWUserInputEventHandler::OnTerminateCommand(BSTR CommandName, NameValueMap * Context)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try{
		if (m_pParent)
			return m_pParent->OnTerminateCommand(CommandName, Context);
	}catch (_com_error e){
		ATLASSERT(0);
		return e.Error();
	}

	return S_OK;
}

STDMETHODIMP CWUserInputEventHandler::OnContextualMiniToolbar(ObjectsEnumerator* SelectedEntities, NameValueMap* DisplayedCommands, NameValueMap* AdditionalInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try{
		if (m_pParent)
			return m_pParent->OnContextualMiniToolbar(SelectedEntities, DisplayedCommands, AdditionalInfo);
	}catch (_com_error e){
		ATLASSERT(0);
		return e.Error();
	}

	return S_OK;
}

HRESULT IUtilUserInputEventHandler::Connect(Application *pApplication,
										   IUtilUserInputEventHandler* pParent,
										   CComObject<CWUserInputEventHandler>** ppHandler)
{
	HRESULT hr = S_OK; 

	CComPtr<CommandManager> pCmdMgr;
	hr = pApplication->get_CommandManager(&pCmdMgr);
	ATLASSERT(SUCCEEDED(hr));

	CComPtr<UserInputEventsObject> pUsrInEvents;
	hr = pCmdMgr->get_UserInputEvents(&pUsrInEvents);
	ATLASSERT(SUCCEEDED(hr));

	return Connect(pUsrInEvents, pParent, ppHandler);
}

HRESULT IUtilUserInputEventHandler::Connect(UserInputEventsObject *pUserInputEvents, 
										   IUtilUserInputEventHandler* pParent,
										   CComObject<CWUserInputEventHandler>** ppHandler)
{
	HRESULT hr = S_OK; 

	//Create the object that contains the event implementation.
	CComObject<CWUserInputEventHandler>* pUserInputEventHandler;
	hr = CComObject<CWUserInputEventHandler>::CreateInstance (&pUserInputEventHandler);
	ATLASSERT(SUCCEEDED(hr));

	//Connect the events.
	hr = pUserInputEventHandler->IDispEventImpl<USERINPUT_EVENTS, CWUserInputEventHandler, &DIID_UserInputEventsSink,
		&LIBID_Inventor, INVENTOR_MAJOR, INVENTOR_MINOR>::DispEventAdvise (pUserInputEvents,&DIID_UserInputEventsSink);
	ATLASSERT(SUCCEEDED(hr));

	pUserInputEventHandler->SetParent(pParent);
	pUserInputEventHandler->SetObject(pUserInputEvents);

	*ppHandler = pUserInputEventHandler;
	(*ppHandler)->AddRef();

	return hr;
}

HRESULT IUtilUserInputEventHandler::Disconnect(CComObject<CWUserInputEventHandler>** ppHandler)
{
	HRESULT hr=S_OK;

	if (*ppHandler)
	{
		IUnknown* pUserInputEvents = (*ppHandler)->GetObject();
		hr = (*ppHandler)->IDispEventImpl<USERINPUT_EVENTS, CWUserInputEventHandler, &DIID_UserInputEventsSink,
			&LIBID_Inventor, INVENTOR_MAJOR, INVENTOR_MINOR>::DispEventUnadvise (pUserInputEvents,&DIID_UserInputEventsSink);
		ATLASSERT(SUCCEEDED(hr));

		(*ppHandler)->Release();
		(*ppHandler) = NULL;
	}

  return hr;
}

