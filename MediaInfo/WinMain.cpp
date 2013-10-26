//========================================================================================================
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "\
	"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//========================================================================================================
#include <Shlwapi.h>
#include "InfoDlg.h"
//========================================================================================================
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR lpCmdLine, int)
{
	InfoDlg infoDlg;
	MSG msg = {0};

	if (lstrcmp(lpCmdLine, _T(""))!=0)
	{
		TCHAR *p = lpCmdLine;
		static TCHAR szFile[MAX_PATH];// = {0};
		int i = 0;

		while(*p != '\"' && *p != '/')
			szFile[i++] = *p, p++;

		szFile[--i] = 0;
		if (PathIsDirectory(szFile) ||			//Don't deal with folder now
			!infoDlg.IsSupportedFormat(szFile))	//No way with unsupported file
		{
			//MessageBox(0, szFile, L"Unsupported", 0);
			return 0;
		}

		//MessageBox(0, szFile, L"test", 0);

		if (*p == '/')	++p;

		if (*p == 'c')
		{
			TString info = infoDlg.ReadMediaInfo(szFile, false);
			if (!infoDlg.CopyToClipbrd(info.c_str()))
				MessageBox(GetActiveWindow(), _T("∏¥÷∆µΩºÙ«–∞Â ß∞‹£°"), _T("Media Info"), MB_ICONERROR);
		}
		else if (*p == 's')
		{
			TString filePath = infoDlg.SelectFilePath(szFile);
			if (filePath.empty())
				return 0;

			TString info = infoDlg.ReadMediaInfo(szFile, false);
#ifdef UNICODE
			if (!infoDlg.WriteFile(filePath, (LPVOID)U2A(info.c_str()), lstrlenA(U2A(info.c_str()))/**sizeof(TCHAR)*/))
#else
			if (!infoDlg.WriteFile(filePath, (LPVOID)info.c_str(), info.length()*sizeof(TCHAR)))
#endif
				MessageBox(GetActiveWindow(), _T("±£¥Ê ”∆µ–≈œ¢ ß∞‹£°"), _T("Media Info"), MB_ICONERROR);

		}

		return 0;
	}

	try
	{
		infoDlg.init(hInst, NULL);

		//infoDlg.doModal(IDD_MEDIA_INFO);
		infoDlg.create(IDD_MEDIA_INFO);
		HACCEL hAcc = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

		while(GetMessage(&msg, NULL, NULL, NULL))
		{
			if(msg.message == WM_KEYDOWN)
			{
				SendMessage(infoDlg.getSelf(), WM_KEYDOWN, msg.wParam, msg.lParam);
			}
			//else
			{
				if(/*!IsDialogMessage(infoDlg.getSelf(), &msg) &&*/
					!TranslateAccelerator(infoDlg.getSelf(), hAcc, &msg)
					)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
	}
	catch(...)
	{
		MessageBox(GetActiveWindow(), _T("Unexpected error happened!"), _T("Error"), MB_ICONERROR | MB_ICONSTOP);
	}
	return msg.wParam;
}