// MediaInfoShell.cpp : Implementation of CMediaInfoShell

#include "stdafx.h"
#include "MediaInfoShell.h"

STDMETHODIMP CMediaInfoShell::Initialize( LPCITEMIDLIST pidlFolder,
	LPDATAOBJECT pDataObj, HKEY hProgID )
{
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

	// Look for CF_HDROP data in the data object.
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ) ))
	{
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG;
	}

	// Get a pointer to the actual data.
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	// Make sure it worked.
	if ( NULL == hDrop )
		return E_INVALIDARG;

	// Sanity check - make sure there is at least one filename.
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );
	HRESULT hr = S_OK;

	if ( 0 == uNumFiles )
	{
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg );
		return E_INVALIDARG;
	}

	// Get the name of the first file and store it in our member variable m_szFile.
	if ( 0 == DragQueryFile ( hDrop, 0, m_szFile, MAX_PATH ) )
		hr = E_INVALIDARG;

	GlobalUnlock ( stg.hGlobal );
	ReleaseStgMedium ( &stg );

	return hr;
}

STDMETHODIMP CMediaInfoShell::QueryContextMenu (HMENU hmenu, UINT uMenuIndex,
	UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags )
{
	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
	if ( uFlags & CMF_DEFAULTONLY )
		return MAKE_HRESULT( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
	UINT uidCmd = uidFirstCmd;
	InsertMenu(hmenu, uMenuIndex, MF_SEPARATOR | MF_BYPOSITION, 0, NULL); 
	InsertMenu(hmenu, uMenuIndex, MF_BYPOSITION, uidCmd++, _T("复制视频信息到剪切板") );
	InsertMenu(hmenu, uMenuIndex, MF_BYPOSITION, uidCmd++, _T("保存视频信息到TXT") );
	InsertMenu(hmenu, uMenuIndex, MF_SEPARATOR | MF_BYPOSITION, 0, NULL); 

	return MAKE_HRESULT( SEVERITY_SUCCESS, FACILITY_NULL, 2 );
}

STDMETHODIMP CMediaInfoShell::GetCommandString(UINT idCmd, UINT uFlags,
	UINT* pwReserved, LPSTR pszName, UINT cchMax )
{
	USES_CONVERSION;

	TCHAR *pszPromt = NULL;
	switch( idCmd )
	{
	case 0:
		pszPromt = _T("将当前选择的视频的信息复制到剪切板");
		break;
	case 1:
		pszPromt = _T("将当前选择的视频的信息保存到一个TXT文件");
		break;
	}

	// If Explorer is asking for a help string, copy our string into the
	// supplied buffer.
	if ( uFlags & GCS_HELPTEXT )
	{
		if ( uFlags & GCS_UNICODE )
		{
			// We need to cast pszName to a Unicode string, and then use the
			// Unicode string copy API.
			lstrcpynW( (LPWSTR) pszName, T2CW(pszPromt), cchMax );
		}
		else
		{
			// Use the ANSI string copy API to return the help string.
			lstrcpynA( pszName, T2CA(pszPromt), cchMax );
		}

		return S_OK;
	}

	return E_INVALIDARG;
}

STDMETHODIMP CMediaInfoShell::InvokeCommand( LPCMINVOKECOMMANDINFO pCmdInfo )
{
	// If lpVerb really points to a string, ignore this function call and bail out.
	if ( 0 != HIWORD( pCmdInfo->lpVerb ) )
		return E_INVALIDARG;

	TCHAR szExePath[MAX_PATH];
	TCHAR szCmd[MAX_PATH*5] = {0};

	if (!GetMIExePath(szExePath))
		return E_INVALIDARG;

	switch(LOWORD( pCmdInfo->lpVerb))
	{
	case 0:
		wsprintf(szCmd, _T("%s /c"), m_szFile);
		break;

	case 1:
		wsprintf(szCmd, _T("%s /s"), m_szFile);
		break;

	default:
		return E_INVALIDARG;
		break;
	}

	ShellExecute(NULL, _T("open"), szExePath, szCmd, NULL, SW_SHOW);

	return S_OK;
}

bool CMediaInfoShell::GetMIExePath(TCHAR *pszPath)
{
	HKEY hKey;
	long lResult;
	bool fOk = false;

	lResult = ::RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("Software\\MIData"), 0, KEY_READ, &hKey);

	if (lResult != ERROR_SUCCESS)
		goto _exit;

	TCHAR szValue[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH;
	lResult = ::RegQueryValueEx(hKey, _T("Path"), 0, 0, (LPBYTE)szValue, &dwLen);

	if (lResult != ERROR_SUCCESS)
		goto _exit;

	lstrcpy(pszPath, szValue);
	fOk = true;

_exit:
	if(hKey)	RegCloseKey(hKey);
	return fOk;
}