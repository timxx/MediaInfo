//========================================================================================================
#ifndef ABOUT_DLG_H
#define ABOUT_DLG_H

#pragma once
//========================================================================================================
#include "dialog.h"
#include "URLCtrl.h"
//========================================================================================================

class AboutDlg : public Dialog
{
public:
	AboutDlg(){}
	~AboutDlg(){}

protected:
	virtual BOOL CALLBACK runProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitText();

private:
	URLCtrl _qMeLink;
	URLCtrl _tinyLink;
};

#endif