//========================================================================================================
#include "AboutDlg.h"
#include "resource.h"
//========================================================================================================

BOOL CALLBACK AboutDlg::runProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			InitText();
			_qMeLink.init(_hinst, _hWnd);
			_qMeLink.create(HwndFromId(IDS_Q_ME),
				_T("http://wpa.qq.com/msgrd?v=1&uin=551907703&site=qq&menu=yes"));
				//_T("tencent://message/?uin=551907703"));

			_tinyLink.init(_hinst, _hWnd);
			_tinyLink.create(HwndFromId(IDS_TINY), _T("http://www.movie007.com/space-uid-8498.html"));
		}
		return TRUE;

	case WM_CLOSE:
		Destroy();
		break;
	}

	return FALSE;
}

void AboutDlg::InitText()
{
	TCHAR szInfo[] =
		_T("------------------------------使用说明------------------------------\r\n")
		_T("\r\n-打开文件\r\n")
		_T("---直接拖动单个或多个视频文件到窗口\r\n")
		_T("---通过菜单打开文件，可一次打开单个或多个\r\n")
		_T("---通过菜单打开文件夹指定要搜索的文件夹，程序会将其及子文件夹内的视频搜索出来\r\n")
		_T("---在[当前视频]下拉框可切换要查看的视频的信息\r\n")
		_T("\r\n-保存信息\r\n")
		_T("---通过Ctrl+C复制到剪切板再进行处理\r\n")
		_T("---通过菜单单个或批量保存到指定TXT或文件夹内\r\n")
		_T("---找到视频文件，右键在弹出菜单选[复制视频信息到剪切板]则复制到剪切板\r\n")
		_T("---选[保存视频信息到TXT]则保存到指定的TXT文件\r\n")
		_T("\r\n-单击菜单[自定义文字设置]可设置在视频信息后面显示指定的文字\r\n")
		_T("\r\n-注册或移除右键需管理员权限，如果是在Win7且开启了UAC，则需用管理员权限运行，否则不能注册或移除\r\n")		
		_T("\r\nMD5值是根据文件大小计算得出\r\n")
		_T("\r\n\t\t\t\t Just Fancy ( 551907703 )\r\n")
		_T("\t\t\t\t\tAug. 19, 2010")
		;

	SetItemText(IDC_EDIT1, szInfo);
	//SendDlgItemMessage(_hWnd, IDC_EDIT1, EM_SETSEL, 0, 0);
}