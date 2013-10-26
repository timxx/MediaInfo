// MediaInfoShell.h : Declaration of the CMediaInfoShell

#pragma once
#include "resource.h"       // main symbols



#include "MIShell_i.h"
#include <shlobj.h>
//#include <comdef.h>



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CMediaInfoShell

class ATL_NO_VTABLE CMediaInfoShell :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMediaInfoShell, &CLSID_MediaInfoShell>,
	public IDispatchImpl<IMediaInfoShell, &IID_IMediaInfoShell, &LIBID_MIShellLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,	//
	public IContextMenu		//
{
public:
	CMediaInfoShell()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MEDIAINFOSHELL)


BEGIN_COM_MAP(CMediaInfoShell)
	COM_INTERFACE_ENTRY(IMediaInfoShell)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// IShellExtInit
	STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

	// IContextMenu
    STDMETHODIMP GetCommandString(UINT, UINT, UINT*, LPSTR, UINT);
    STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
    STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);

	bool GetMIExePath(TCHAR *pszPath);	//取得主程序路径
protected:
  TCHAR m_szFile[MAX_PATH];
};

OBJECT_ENTRY_AUTO(__uuidof(MediaInfoShell), CMediaInfoShell)
