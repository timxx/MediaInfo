//========================================================================================================
#include <shlobj.h>
#include <Shlwapi.h>

#include "InfoDlg.h"
#include "MediaInfoDLL_Static.h"
#include "md5.h"
#include "CustomTextDlg.h"
#include "AboutDlg.h"
//========================================================================================================
#pragma comment(lib, "MediaInfo.lib")
#pragma comment(lib, "Shlwapi.lib")
//========================================================================================================
DWORD WINAPI SearchThread(LPVOID lParam)
{
	InfoDlg *pInfoDlg = (InfoDlg*)lParam;

	pInfoDlg->beginSearch();

	return 1;
}
//========================================================================================================
#define IDM_MENU_BASE	3000
BOOL CALLBACK InfoDlg::runProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetDlgIcon(IDI_APP);
		return TRUE;

	case WM_DROPFILES:
		OnDropFiles((HDROP)wParam);
		return FALSE;

	case WM_COMMAND:
		OnCommand(LOWORD(wParam), HWND(lParam), HIWORD(wParam));
		break;

	case WM_KEYDOWN:
		OnKeyDown(wParam, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam));
		break;

	case WM_CLOSE:
		Destroy();
		return FALSE;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

void InfoDlg::OnDropFiles(HDROP hDrop)
{
	int count = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

	int prvCount = videoPath.size();

	for (int i= 0; i<count; i++)
	{
		int len = ::DragQueryFile(hDrop, i, NULL, 0);

		TCHAR *filePath = new TCHAR[len+1];

		::DragQueryFile(hDrop, i, filePath, len+1);

		if (IsSupportedFormat(filePath) && !IsFileIncluded(filePath))
		{
			VideoPathInfo vPath(filePath);
			videoPath.push_back(vPath);

			AddToComboBox(vPath.fileTitle);
		}
		delete [] filePath;
	}

	if (prvCount != videoPath.size() && videoPath.size() != 0)
	{
		::SendDlgItemMessage(_hWnd, IDC_VIDEO_TITLE, CB_SETCURSEL, prvCount, 0);
		ReadMediaInfo(videoPath[prvCount].filePath.c_str());
	}

	::DragFinish(hDrop);
}

void InfoDlg::OnKeyDown(UINT vk, int cRepeat, UINT flags)
{
	//if (vk == VK_RETURN)
	//	MsgBox(L"test", L"dwown");
}

void InfoDlg::OnCommand(int id, HWND hwndCtl, UINT codeNotify)
{
	if (codeNotify == CBN_SELCHANGE)
	{
		int curSel = ::SendDlgItemMessage(_hWnd, IDC_VIDEO_TITLE, CB_GETCURSEL, 0, 0);
		if (videoPath.size() > curSel)
			ReadMediaInfo(videoPath[curSel].filePath.c_str());
	}

	switch(id)
	{
	case IDM_OPEN:
		doOpenFile();
		break;
		
	case IDM_FOLDER:
		doOpenFolder();
		break;

	case IDM_SAVE_CUR:
		{
			int curSel = ::SendDlgItemMessage(_hWnd, IDC_VIDEO_TITLE, CB_GETCURSEL, 0, 0);
			if (curSel != -1 && curSel < videoPath.size())
			{
				TString filePath = SelectFilePath(videoPath[curSel].filePath.c_str());
				if (!filePath.empty())
				{
					TString info = ReadMediaInfo(videoPath[curSel].filePath.c_str(), false);
#ifdef UNICODE
					if (!WriteFile(filePath, (LPVOID)U2A(info.c_str()), lstrlenA(U2A(info.c_str()))/**sizeof(TCHAR)*/))
#else
					if (!WriteFile(filePath, (LPVOID)info.c_str(), info.length()*sizeof(TCHAR)))
#endif

						MsgBox(_T("保存失败！"), _T("出错了"), MB_ICONERROR);
				}
			}
		}
		break;

	case IDM_SAVE_SAME:
		{
			if (videoPath.size() <= 0)
				break;

			TString filePath = SelectFilePath();
			if (!filePath.empty())
			{
				if (!AllInfoToSingleFile(filePath))
					MsgBox(_T("写入文件失败！！！"), _T("出错了"), MB_ICONERROR);
			}
		}
		break;

	case IDM_SAVE_DIFF:
		{
			int count = videoPath.size();
			if (count <= 0)	break;

			TString folder = SelectAFolder(_T("请选择一个文件夹存放导出的TXT"));

			if (folder.empty())
			{
				bool fOk = true;
				for (int i=0; i<count; i++)
				{
					TString info = ReadMediaInfo(videoPath[i].filePath.c_str(), false);

					TCHAR filePath[MAX_PATH];
					lstrcpy(filePath, videoPath[i].filePath.c_str());
					lstrcpy(_tcsrchr(filePath, '.')+1, _T("txt"));

#ifdef UNICODE
					if (!WriteFile(filePath, (LPVOID)U2A(info.c_str()), lstrlenA(U2A(info.c_str()))/**sizeof(TCHAR)*/))
#else
					if (!WriteFile(filePath, (LPVOID)info.c_str(), info.length()*sizeof(TCHAR)))
#endif
					{
						fOk = false;
						MsgBox(_T("保存失败！"), _T("出错了"), MB_ICONERROR);
					}
				}
				if(fOk)
					MsgBox(_T("视频信息已分别保存为与对应视频同名的TXT文件！"), _T("保存完成"), MB_ICONINFORMATION);
			}
			else
			{
				bool fOk = true;
				for (int i=0; i<count; i++)
				{
					TString info = ReadMediaInfo(videoPath[i].filePath.c_str(), false);

					TCHAR filePath[MAX_PATH];

					wsprintf(filePath, _T("%s\\%s"), folder.c_str(), videoPath[i].fileTitle.c_str());
					lstrcpy(_tcsrchr(filePath, '.')+1, _T("txt"));

#ifdef UNICODE
					if (!WriteFile(filePath, (LPVOID)U2A(info.c_str()), lstrlenA(U2A(info.c_str()))/**sizeof(TCHAR)*/))
#else
					if (!WriteFile(filePath, (LPVOID)info.c_str(), info.length()*sizeof(TCHAR)))
#endif
					{
						fOk = false;
						MsgBox(_T("保存失败！"), _T("出错了"), MB_ICONERROR);
					}
				}
				if (fOk)
					MsgBox(_T("视频信息已全部导出到TXT！"), _T("信息"), MB_ICONINFORMATION);
			}

		}
		break;

	case IDM_COPY:
		{
			int nCurSel = ::SendDlgItemMessage(_hWnd, IDC_VIDEO_TITLE, CB_GETCURSEL, 0, 0);
			if (nCurSel == -1)	break ;

			TString info = ReadMediaInfo(videoPath[nCurSel].filePath.c_str(), false);

			if (!CopyToClipbrd(info.c_str()))
				MsgBox(_T("复制到剪切板失败！"), _T("出错了"), MB_ICONERROR);
		}
		break;

	case IDM_REGISTER:
		{
			if (!WritePathToReg())
				MsgBox(_T("写注册表失败，这会导致右键功能无法使用！！！"),
				_T("出错了"), MB_ICONERROR);
			int result = RegisterOpenWith();
			if (result != 0)
			{
				if (result == ERROR_ACCESS_DENIED)
				{
					MsgBox(_T("开启了UAC的Win7系统请以管理员权限来运行程序注册右键！！！"),
						_T("注册失败"), MB_ICONERROR);
				}
				else if (result == ERROR_FILE_NOT_FOUND)
				{
					MsgBox(_T("请确保MIShell.dll存在于程序所在目录！"), _T("DLL丢失"), MB_ICONERROR);
				}
			}
		}
		break;

	case IDM_UNREGISTER:
		{
			int result = RegisterOpenWith(false);
			if (result != 0)
			{
				if (result == ERROR_ACCESS_DENIED)
				{
					MsgBox(_T("开启了UAC的Win7系统请以管理员权限来运行程序注册右键！！！"),
						_T("解除注册右键失败"), MB_ICONERROR);
				}
				else if (result == ERROR_FILE_NOT_FOUND)
				{
					MsgBox(_T("请确保MIShell.dll存在于程序所在目录！"), _T("DLL丢失"), MB_ICONERROR);
				}
			}
		}
		break;

	case IDM_CUSTOMTEXT_SET:
		{
			CustomTextDlg ctDlg;
			ctDlg.init(_hinst, _hWnd);
			ctDlg.doModal(IDD_CUSTOMTEXT);
		}
		break;

	case IDM_ABOUT:
		{
			AboutDlg aboutDlg;
			aboutDlg.init(_hinst, _hWnd);
			aboutDlg.doModal(IDD_ABOUT);
		}
		break;

	case IDM_EXIT:
		SendMsg(WM_CLOSE);
		break;
	}
}

void InfoDlg::AddToComboBox(const TString &fileTitle)
{
	::SendDlgItemMessage(_hWnd, IDC_VIDEO_TITLE, CB_ADDSTRING, 0, (LPARAM)fileTitle.c_str());
}

TString InfoDlg::ReadMediaInfo(const TCHAR *filePath, bool fUpdateToDlg/* = true*/)
{
	HCURSOR hCurOld = SetCursor(LoadCursor(NULL, IDC_WAIT));

	HANDLE handle = MediaInfo_New();

	MediaInfo_Open(handle, filePath);
	TCHAR buffer[MAX_PATH];

	TString info(_T("---------------------------------------------------\r\n"));
	const TCHAR *msg = MediaInfo_Get(handle, MediaInfo_Stream_General, 0, _T("FileName"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("文件名: ");
	info += msg;

	lstrcpy(buffer, msg);
	msg = MediaInfo_Get(handle, MediaInfo_Stream_General, 0, _T("FileExtension"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T(".");
	info += msg;
	info +=  _T("\r\n");

	lstrcat(buffer, _T("."));
	lstrcat(buffer, msg);
	if (fUpdateToDlg)
		SetItemText(IDS_NAME, buffer);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_General, 0, _T("FileSize/String4"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("大小: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_SIZE, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_General, 0, _T("FileSize"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("MD5：");
	MD5 md5;
#ifdef UNICODE
	md5.update(U2A(msg));
	msg = _wcsupr(A2U(md5.toString().c_str()));
#else
	md5.update(msg);
	msg = strupr((char*)md5.toString().c_str());
#endif
	if (fUpdateToDlg)
		SetItemText(IDS_MD5, msg);
	info += msg;
	info +=  _T("\r\n");

	info +=  _T("分辨率: ");
	msg = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, _T("Width"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += msg;
	info += _T(" * ");
	lstrcpy(buffer, msg);
	lstrcat(buffer, _T(" * "));
	msg = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, _T("Height"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += msg;
	info +=  _T("\r\n");

	lstrcat(buffer, msg);
	if (fUpdateToDlg)
		SetItemText(IDS_RESOLUTION, buffer);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_General, 0, _T("Duration/String3"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("时长: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_DURATION, msg);


	msg = MediaInfo_Get(handle, MediaInfo_Stream_General, 0, _T("OverallBitRate/String"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("码率: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_BITRATE, msg);

	info += _T("---------------------------------------------------\r\n");
	info += _T("视频信息\r\n\r\n");
	msg = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, _T("Format"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("格式: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_VFORMAT, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, _T("Codec"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("编码: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_VCODEC, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, _T("CodecID/Hint"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("编码提示: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_VCODEC_HINT, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, _T("BitRate/String"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("码率: ";)
		info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_VRATE, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, _T("FrameRate/String"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("帧率: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_VFRAMERATE, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Video, 0, _T("DisplayAspectRatio/String"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("高宽比: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_VDAR, msg);

	info += _T("---------------------------------------------------\r\n");
	info += _T("音频信息\r\n\r\n");

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, _T("Format"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("格式: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_AFORMAT, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, _T("Codec"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("编码: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_ACODEC, msg);


	msg = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, _T("CodecID/Hint"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("编码提示: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_ACODEC_HINT, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, _T("BitRate/String"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("码率: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_ARATE, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, _T("Format_Settings_Mode"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("模式: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_AMODE, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, _T("Format_Settings_ModeExtension"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("扩展模式: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_AEXMODE, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, _T("SamplingRate/String"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("取样速率: ");
	info += msg;
	info +=  _T("\r\n");
	if (fUpdateToDlg)
		SetItemText(IDS_ASAMPLERATE, msg);

	msg = MediaInfo_Get(handle, MediaInfo_Stream_Audio, 0, _T("Channel(s)/String"), MediaInfo_Info_Text, MediaInfo_Info_Name);
	info += _T("通道: ");
	info += msg;
	info += _T("\r\n---------------------------------------------------\r\n");

	if (fUpdateToDlg)
		SetItemText(IDS_ACHANNEL, msg);

	if (GetCustomText(buffer))
	{
		info += buffer;
		info += _T("\r\n===================================================\r\n");
	}

	MediaInfo_Close(handle);

	SetCursor(hCurOld);

	return info;
}

bool InfoDlg::CopyToClipbrd(LPCTSTR lpData)
{
	if (!::OpenClipboard(_hWnd))
		return false;
	bool fOk = false;
	EmptyClipboard();

	int len = lstrlen(lpData);//info.length();

	if (len > 0)
	{
		HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(TCHAR));

		if (hglbCopy == NULL)
			goto _exit;

		LPTSTR  lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);

		memcpy(lptstrCopy, lpData, len * sizeof(TCHAR));

		GlobalUnlock(hglbCopy);
		UINT format = CF_UNICODETEXT;

#ifndef UNICODE
		format = CF_TEXT;
#endif
		if (SetClipboardData(format, hglbCopy)==NULL)
			goto _exit;
	}
	fOk = true;
_exit:
	CloseClipboard();

	return fOk;
}

bool InfoDlg::AllInfoToSingleFile(TString filePath)
{
	HANDLE hFile;
	bool fOk = false;
	int count = videoPath.size();
	if (count <= 0) return true;

	hFile = ::CreateFile(filePath.c_str(), GENERIC_WRITE,  0,\
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!hFile)	return false;

	DWORD dwWritten;

	for (int i=0; i<count; i++)
	{
		TString info = ReadMediaInfo(videoPath[i].filePath.c_str(), false);
#ifdef UNICODE
		if (!::WriteFile(hFile, (LPVOID)U2A(info.c_str()), lstrlenA(U2A(info.c_str()))/* * sizeof(TCHAR)*/, &dwWritten, NULL))
#else
		if (!::WriteFile(hFile, (LPVOID)info.c_str(), info.length() * sizeof(TCHAR), &dwWritten, NULL))
#endif
			goto _exit;
	}

	fOk = true;
_exit:
	if (hFile)	CloseHandle(hFile);
	return fOk;
}

bool InfoDlg::WriteFile(TString filePath, LPVOID lpData, DWORD dwLen)
{
	HANDLE hFile;
	bool fOk = false;

	hFile = ::CreateFile(filePath.c_str(), GENERIC_WRITE,  0,\
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!hFile)	return false;

	DWORD dwWritten;

	if (!::WriteFile(hFile, lpData, dwLen, &dwWritten, NULL))
		goto _exit;

	fOk = true;
_exit:
	if (hFile)	CloseHandle(hFile);
	return fOk;

}

TString InfoDlg::SelectFilePath(const TCHAR *fileToSave)
{
	OPENFILENAME	ofn;
	SYSTEMTIME		st;
	TCHAR filePath[MAX_PATH] = {0};

	::RtlSecureZeroMemory(&ofn, sizeof(ofn));
	::RtlSecureZeroMemory(&st, sizeof(SYSTEMTIME));

	if (fileToSave != NULL)
	{
		VideoPathInfo vpi(fileToSave);
		lstrcpy(filePath, vpi.fileTitle.c_str());
		lstrcpy(_tcsrchr(filePath, '.')+1, _T("txt"));
	}

	ofn.hInstance 	 = _hinst;
	ofn.hwndOwner 	 = _hWnd;
	ofn.Flags 		 = OFN_OVERWRITEPROMPT;
	ofn.lpstrFile	 = filePath;
	ofn.nMaxFile 	 = MAX_PATH;
	ofn.lStructSize  = sizeof(OPENFILENAME);
	ofn.lpstrFilter  = _T("TXT(*.txt)\0*.txt\0\0");
	ofn.lpstrTitle   = _T("保存视频信息");

	if (::GetSaveFileName(&ofn))
	{
		TCHAR *ext = PathFindExtension(filePath);
		if (lstrcmpi(ext, _T("")) == 0)
			lstrcat(filePath, _T(".txt"));
		return filePath;
	}

	return _T("");
}

TString InfoDlg::SelectAFolder(const TCHAR *title)
{
	BROWSEINFO   bi;       
	TCHAR	dispName[MAX_PATH],  pFileName[MAX_PATH];       
	ITEMIDLIST   *   pidl;       

	bi.hwndOwner		 =   _hWnd;       
	bi.pidlRoot			 =   0;       
	bi.pszDisplayName    =   dispName;       
	bi.lpszTitle		 =   title;       
	bi.ulFlags			 =   BIF_RETURNONLYFSDIRS | BIF_EDITBOX;       
	bi.lpfn				 =   0;       
	bi.lParam			 =   0;       
	bi.iImage			 =   0;       

	pidl = ::SHBrowseForFolder(&bi);
	if (pidl == NULL)
		return _T("");

	if (!::SHGetPathFromIDList(pidl, pFileName))
		return _T("");

	return pFileName;
}

void InfoDlg::doOpenFile()
{
	OPENFILENAME ofn;
	TCHAR filePath[MAX_PATH*10] = {0};

	::RtlSecureZeroMemory(&ofn, sizeof (ofn) );

	ofn.hInstance 	= _hinst;
	ofn.hwndOwner 	= _hWnd;
	ofn.Flags 		= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST |\
		OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FORCESHOWHIDDEN;
	ofn.lpstrFile 	= filePath;
	ofn.nMaxFile 	= MAX_PATH*10;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = _T("All files(*.*)\0*.*\0\0");
	ofn.lpstrTitle 	= _T("请选择视频文件");

	TCHAR *p = NULL;
	if (GetOpenFileName(&ofn))
		p = filePath;

	if (!p)	return;

	TCHAR szFile[MAX_PATH];
	int prevCount = videoPath.size();

	lstrcpy(szFile, filePath);

	while(*p++)
		;
	if (*p == NULL) //只选了一个文件
	{
		if (IsSupportedFormat(szFile) && !IsFileIncluded(filePath))
		{
			VideoPathInfo vPath(szFile);
			videoPath.push_back(vPath);

			AddToComboBox(vPath.fileTitle);
		}
	}
	else	//选择了多个文件
	{
		while (*p)
		{
			lstrcpy(szFile, filePath);
			lstrcat(szFile, _T("\\"));
			lstrcat(szFile, p);

			if (IsSupportedFormat(szFile) && !IsFileIncluded(filePath))
			{
				VideoPathInfo vPath(szFile);
				videoPath.push_back(vPath);
				
				AddToComboBox(vPath.fileTitle);
			}

			while (*p++)
				;
		}
	}
	if (prevCount != videoPath.size())
	{
		::SendDlgItemMessage(_hWnd, IDC_VIDEO_TITLE, CB_SETCURSEL, prevCount, 0);
		ReadMediaInfo(videoPath[prevCount].filePath.c_str());
	}
}

void InfoDlg::doOpenFolder()
{
	TString folder = SelectAFolder(_T("请选择一个包含有视频的文件夹"));
	if (folder.empty())	return ;

	_folderForSearching = folder;

	DWORD dwID;
	HANDLE hThread = CreateThread(NULL, 0, SearchThread, this, 0, &dwID);
	if (!hThread)
	{
		MsgBox(_T("创建线程失败！"), _T("错误"), MB_ICONERROR);
	}
	MsgBox(_T("正在搜索支持的视频文件……\r\n您可以关闭此消息框进行后台搜索。"),
		_T("信息"), MB_ICONINFORMATION);
}
void InfoDlg::beginSearch()
{
	int prvCount = videoPath.size();

	if (_folderForSearching.empty())
		return ;

	iFile = 0;
	int count = SearchVideo(_folderForSearching.c_str());

	if (prvCount != videoPath.size())
	{
		::SendDlgItemMessage(_hWnd, IDC_VIDEO_TITLE, CB_SETCURSEL, prvCount, 0);
		ReadMediaInfo(videoPath[prvCount].filePath.c_str());
	}

	TCHAR info[100];

	wsprintf(info, _T("在[%s]文件夹（及子文件夹）下共搜到%d个支持的视频（不计重复文件）。"),
		_folderForSearching.c_str(), count);

	MsgBox(info, _T("搜索完成"), MB_ICONINFORMATION);
}

int InfoDlg::SearchVideo(const TCHAR * folder)
{
	int len = lstrlen(folder);
	TCHAR tmp[MAX_PATH];
	lstrcpy(tmp, folder);
	TCHAR cc = folder[len-1];
	
	if ( cc != '\\')
		lstrcat(tmp, _T("\\*"));
	else if (cc == '\\')
		lstrcat(tmp, _T("*"));

	WIN32_FIND_DATA wfd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = ::FindFirstFile(tmp, &wfd);

	if (!hFind)	return iFile;

	do
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(wfd.cFileName, _T("."))==0 ||
				lstrcmp(wfd.cFileName, _T(".."))==0	)

				continue;

			TString fullPath = folder;

			if ( cc != '\\')
				fullPath += _T("\\");

			fullPath += wfd.cFileName;

			SearchVideo(fullPath.c_str());
		}
		else
		{
			TString fullPath = folder;

			if ( cc != '\\')
				fullPath += _T("\\");

			fullPath += wfd.cFileName;

			if (IsSupportedFormat(fullPath.c_str()) && !IsFileIncluded(fullPath.c_str()))
			{
				VideoPathInfo vPath(fullPath);
				videoPath.push_back(vPath);

				AddToComboBox(vPath.fileTitle);

				iFile ++;
			}
		}

	}while(FindNextFile(hFind, &wfd));

	return iFile;
}

int InfoDlg::RegisterOpenWith(bool fRegister/* = true*/)
{
	TCHAR dllPath[MAX_PATH];
	GetModuleFileName(_hinst, dllPath, MAX_PATH);
	lstrcpy(_tcsrchr(dllPath, '\\') + 1, _T("MIShell.dll"));

	if (!PathFileExists(dllPath))
	{
		return ERROR_FILE_NOT_FOUND;
	}
	TCHAR szParam[MAX_PATH];

	if (fRegister)
		wsprintf(szParam, _T("\"%s\""), dllPath);
	else
		wsprintf(szParam, _T("/u \"%s\""), dllPath);
	
	if ((int)ShellExecute(_hWnd, _T("open"), _T("regsvr32.exe"),
		szParam, NULL, SW_SHOW) == ERROR_ACCESS_DENIED)
		return ERROR_ACCESS_DENIED ;

	return 0;
}

bool InfoDlg::GetCustomText(TCHAR *pszText)
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

bool InfoDlg::IsSupportedFormat(const TCHAR *filePath)
{
	TCHAR *szSupportedFmts[] = {
		_T(".mkv"),	 _T(".mka"),  _T(".mks"), _T(".ogg"), _T(".ogm"),
		_T(".avi"),  _T(".mpeg"), _T(".mpg"), _T(".vob"), _T(".mp4"),
		_T(".mpgv"), _T(".mpv"),  _T(".m1v"), _T(".m2v"), _T(".asf"),
		_T(".wmv"),  _T(".qt"),   _T(".mov"), _T(".rm"),  _T(".rmvb"),
		_T(".ra"),   _T(".ifo"),  _T(".dat"), _T(".flv"), _T(".3gp"),
		_T(".m4v")
	};

	TCHAR *ext = PathFindExtension(filePath);
	//TCHAR *ext = _tcsrchr((TCHAR*)filePath, '.');

	for(int i=0; i<26; i++)
		if (lstrcmpi(szSupportedFmts[i], ext) == 0)
			return true;

	return false;
}

bool InfoDlg::IsFileIncluded(const TCHAR *filePath)
{
	int count = videoPath.size();

	for (int i=0; i<count; i++)
		if (lstrcmpi(videoPath[i].filePath.c_str(), filePath)==0)
			return true;
	return false;
}

bool InfoDlg::WritePathToReg()
{
	HKEY hKey;
	long lResult;
	bool fOk = false;

	lResult = ::RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\MIData"),
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hKey, 0);

	if (lResult != ERROR_SUCCESS)
		goto _exit;

	TCHAR exePath[MAX_PATH];
	GetModuleFileName(_hinst, exePath, MAX_PATH);

	lResult = RegSetValueEx(hKey, _T("Path"), 0, REG_SZ, (const BYTE*)exePath, lstrlen(exePath)*sizeof(TCHAR));

	if (lResult!=ERROR_SUCCESS)
		goto _exit;

	fOk = true;

_exit:
	if (hKey)	::RegCloseKey(hKey);
	return fOk;
}