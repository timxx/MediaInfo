//========================================================================================================
#include "CustomTextDlg.h"
#include "resource.h"
//========================================================================================================

BOOL CALLBACK CustomTextDlg::runProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			SendMsg(DM_SETDEFID, IDC_BUTTON1, 0);
			SetLimitText(IDC_EDIT1, 52);
			TCHAR szText[52];
			if (!GetCustomText(szText))
				SetItemText(IDC_EDIT1, _T("此处输入你想要在视频信息后面显示的文字"));
			else
				SetItemText(IDC_EDIT1, szText);
		}
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1)
		{
			TString text = GetItemText(IDC_EDIT1);
			if (!text.empty() && text != _T("此处输入你想要在视频信息后面显示的文字"))
				WriteCustomText((TCHAR*)text.c_str());

			SendMsg(WM_CLOSE);
		}
		break;

	case WM_CLOSE:
		Destroy();
		break;
	}
	return FALSE;
}

bool CustomTextDlg::WriteCustomText(TCHAR *pszText)
{
	TCHAR iniFile[MAX_PATH];
	GetModuleFileName(_hinst, iniFile, MAX_PATH);
	lstrcpy(_tcsrchr(iniFile, '\\') + 1, _T("config.ini"));

	return ::WritePrivateProfileString(_T("MediaIno"), _T("Custom_Text"), pszText, iniFile);
}

bool CustomTextDlg::GetCustomText(TCHAR *pszText)
{
	TCHAR iniFile[MAX_PATH];
	GetModuleFileName(_hinst, iniFile, MAX_PATH);
	lstrcpy(_tcsrchr(iniFile, '\\') + 1, _T("config.ini"));

	TCHAR text[52];
	if (!GetPrivateProfileString(_T("MediaIno"), _T("Custom_Text"), 
		_T(""), text, 52, iniFile))
		return false;

	if (pszText != NULL)
		lstrcpy(pszText, text);
	return true;
}
