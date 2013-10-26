//========================================================================================================
#ifndef INFO_DLG_H
#define INFO_DLG_H
//========================================================================================================
#pragma once
//========================================================================================================
#include "dialog.h"
#include "resource.h"

#include <vector>
//========================================================================================================
struct VideoPathInfo
{
	VideoPathInfo(){}
	VideoPathInfo(const TString &path)
	{
		filePath = path;
		int len = ::GetFileTitle(path.c_str(), NULL, 0);
		TCHAR *buf = new TCHAR[len];
		::GetFileTitle(path.c_str(), buf, len);
		fileTitle = buf;
		delete [] buf;
	}

	TString filePath;
	TString fileTitle;
};
//========================================================================================================
class InfoDlg : public Dialog
{
public:
	InfoDlg(){}
	~InfoDlg(){}

protected:
	virtual BOOL CALLBACK runProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnDropFiles(HDROP hDrop);
	void OnKeyDown(UINT vk, int cRepeat, UINT flags);
	void OnCommand(int id, HWND hwndCtl, UINT codeNotify);

	void AddToComboBox(const TString &fileTitle);	//增加视频标题到下拉框
	bool AllInfoToSingleFile(TString filePath);

	TString SelectAFolder(const TCHAR *title);	//选择一个文件夹
	void doOpenFile();
	void doOpenFolder();
	int SearchVideo(const TCHAR * folder);
	int RegisterOpenWith(bool fRegister = true);

	bool GetCustomText(TCHAR *pszText);
	bool IsFileIncluded(const TCHAR *filePath);

	bool WritePathToReg();

public:
	TString ReadMediaInfo(const TCHAR *filePath, bool fUpdateToDlg = true);
	bool CopyToClipbrd(LPCTSTR lpData);
	bool WriteFile(TString filePath, LPVOID lpData, DWORD dwLen);
	TString SelectFilePath(const TCHAR *fileToSave = NULL);
	bool IsSupportedFormat(const TCHAR *filePath);

	void beginSearch();
private:
	TString _folderForSearching;
	std::vector<VideoPathInfo> videoPath;

	int iFile;

};

#endif