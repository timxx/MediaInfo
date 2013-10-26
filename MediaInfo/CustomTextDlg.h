//========================================================================================================
#ifndef CUSTOM_TEXT_DLG_H
#define CUSTOM_TEXT_DLG_H
//========================================================================================================
#pragma once
//========================================================================================================
#include "dialog.h"
//========================================================================================================
class CustomTextDlg : public Dialog
{
public:
	CustomTextDlg(){}
	~CustomTextDlg(){}

protected:
	virtual BOOL CALLBACK runProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool WriteCustomText(TCHAR *pszText);
	bool GetCustomText(TCHAR *pszText);

};

#endif