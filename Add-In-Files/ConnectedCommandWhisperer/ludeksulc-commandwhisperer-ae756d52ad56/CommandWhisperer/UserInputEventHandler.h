#pragma once
#define USERINPUT_EVENTS 0
#define INVENTOR_MAJOR 1
#define INVENTOR_MINOR 0

class IUtilUserInputEventHandler;


// TBD: 4265 followup
#pragma warning (push)
#pragma warning (disable : 4265)  // disable the missing virtual dtor warning

/////////////////////////////////////////////////////////////////////////////
// CUserInputEventHandler

class CWUserInputEventHandler : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispEventImpl< 
				   USERINPUT_EVENTS, 
				   CWUserInputEventHandler,
				   &DIID_UserInputEventsSink,
				   &LIBID_Inventor,
				   INVENTOR_MAJOR,
				   INVENTOR_MINOR
				   >
{
public:
	CWUserInputEventHandler() 
	{
		m_pParent = NULL;
		m_pEventObject = NULL;
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWUserInputEventHandler)
END_COM_MAP()

public:
	//UserInput events sink
	// NOTE: These functions MUST be __stdcall 
	STDMETHOD (OnStartCommand)(CommandIDEnum CommandID);
	STDMETHOD (OnStopCommand)( CommandIDEnum CommandID);
	STDMETHOD (OnContextMenuOld)( SelectionDeviceEnum SelectionDevice,  NameValueMap * AdditionalInfo, 
							   CommandBar * CommandBar);
	STDMETHOD (OnContextMenuDelete)( SelectionDeviceEnum SelectionDevice,  NameValueMap * AdditionalInfo, 
							   CommandBar * CommandBar);
	STDMETHOD (OnDrag)(DragStateEnum DragState, ShiftStateEnum ShiftKeys, Point * ModelPosition, 
					   Point2d * ViewPosition, View * View, NameValueMap * AdditionalInfo, 
					   HandlingCodeEnum * HandlingCode );
	STDMETHOD (OnActivateCommand)(BSTR CommandName, NameValueMap * Context);
	STDMETHOD (OnTerminateCommand)(BSTR CommandName, NameValueMap * Context);
	STDMETHOD (OnContextualMiniToolbar)(ObjectsEnumerator* SelectedEntities, NameValueMap* DisplayedCommands, NameValueMap* AdditionalInfo);

BEGIN_SINK_MAP( CWUserInputEventHandler )
	SINK_ENTRY_EX( USERINPUT_EVENTS, DIID_UserInputEventsSink, UserInputEventsSink_OnStartCommand, OnStartCommand)
	SINK_ENTRY_EX( USERINPUT_EVENTS, DIID_UserInputEventsSink, UserInputEventsSink_OnStopCommand, OnStopCommand)
	SINK_ENTRY_EX( USERINPUT_EVENTS, DIID_UserInputEventsSink, UserInputEventsSink_OnContextMenuOld, OnContextMenuOld)
	SINK_ENTRY_EX( USERINPUT_EVENTS, DIID_UserInputEventsSink, UserInputEventsSink_OnContextMenu, OnContextMenuDelete)
	SINK_ENTRY_EX( USERINPUT_EVENTS, DIID_UserInputEventsSink, UserInputEventsSink_OnDrag, OnDrag)
	SINK_ENTRY_EX( USERINPUT_EVENTS, DIID_UserInputEventsSink, UserInputEventsSink_OnActivateCommand, OnActivateCommand)
	SINK_ENTRY_EX( USERINPUT_EVENTS, DIID_UserInputEventsSink, UserInputEventsSink_OnTerminateCommand, OnTerminateCommand)	
	SINK_ENTRY_EX( USERINPUT_EVENTS, DIID_UserInputEventsSink, UserInputEventsSink_OnContextualMiniToolbar, OnContextualMiniToolbar)
END_SINK_MAP()

private:
	IUtilUserInputEventHandler* m_pParent;
	CComPtr<UserInputEventsObject> m_pEventObject;

public:
	void SetParent(IUtilUserInputEventHandler* pParent) { m_pParent = pParent; }
	UserInputEventsObject* GetObject() { return m_pEventObject; }
	void SetObject(UserInputEventsObject* pObject) { m_pEventObject = pObject; }

};
#pragma warning (pop)  // re-enable warning 4265 for missing virtual dtor


/////////////////////////////////////////////////////////////////////////////
// IUtilUserInputEventHandler

class IUtilUserInputEventHandler 
{
public:
	virtual HRESULT OnStartCommand(CommandIDEnum CommandID)
	{
	    UNREFERENCED_PARAMETER(CommandID);
	    return S_OK;
	}
	virtual HRESULT OnStopCommand( CommandIDEnum CommandID)
	{
	    UNREFERENCED_PARAMETER(CommandID);
	    return S_OK;
	}
	virtual HRESULT OnContextMenuOld( SelectionDeviceEnum SelectionDevice,  NameValueMap * AdditionalInfo, 
							          CommandBar * CommandBar)
	{
	    UNREFERENCED_PARAMETER(SelectionDevice);
	    UNREFERENCED_PARAMETER(AdditionalInfo);
	    UNREFERENCED_PARAMETER(CommandBar);
	    return S_OK;
	}
	virtual HRESULT OnContextMenuDelete( SelectionDeviceEnum SelectionDevice,  NameValueMap * AdditionalInfo, 
							             CommandBar * CommandBar)
	{
	    UNREFERENCED_PARAMETER(SelectionDevice);
	    UNREFERENCED_PARAMETER(AdditionalInfo);
	    UNREFERENCED_PARAMETER(CommandBar);
	    return S_OK;
	}
	virtual HRESULT OnDrag(DragStateEnum DragState, ShiftStateEnum ShiftKeys, Point * ModelPosition, 
					   Point2d * ViewPosition, View * View, NameValueMap * AdditionalInfo, 
					   HandlingCodeEnum * HandlingCode )
	{
	    UNREFERENCED_PARAMETER(DragState);
	    UNREFERENCED_PARAMETER(ShiftKeys);
	    UNREFERENCED_PARAMETER(ModelPosition);
	    UNREFERENCED_PARAMETER(ViewPosition);
	    UNREFERENCED_PARAMETER(View);
	    UNREFERENCED_PARAMETER(AdditionalInfo);
	    UNREFERENCED_PARAMETER(HandlingCode);
	    return S_OK;
	}
	virtual HRESULT OnActivateCommand(BSTR CommandName, NameValueMap * Context)
	{
	    UNREFERENCED_PARAMETER(CommandName);
	    UNREFERENCED_PARAMETER(Context);
	    return S_OK;
	}
	virtual HRESULT OnTerminateCommand(BSTR CommandName, NameValueMap * Context)
	{
	    UNREFERENCED_PARAMETER(CommandName);
	    UNREFERENCED_PARAMETER(Context);
	    return S_OK;
	}
	virtual HRESULT OnContextualMiniToolbar(ObjectsEnumerator* SelectedEntities, NameValueMap* DisplayedCommands, NameValueMap* AdditionalInfo)
	{
	    UNREFERENCED_PARAMETER(SelectedEntities);
	    UNREFERENCED_PARAMETER(DisplayedCommands);
	    UNREFERENCED_PARAMETER(AdditionalInfo);
	    return S_OK;
	}
	static HRESULT Connect (Application *pApplication, 
							IUtilUserInputEventHandler* pParent,
							CComObject<CWUserInputEventHandler>** ppHandler);
						
	static HRESULT Connect (UserInputEventsObject *pUserInputEvents, 
							IUtilUserInputEventHandler* pParent,
							CComObject<CWUserInputEventHandler>** ppHandler);
						
	static HRESULT Disconnect (CComObject<CWUserInputEventHandler>** ppHandler);
	// Prevent the case “destructor of the derived class is not executed because the base class has no virtual destructor”
	virtual ~IUtilUserInputEventHandler(){}
};
