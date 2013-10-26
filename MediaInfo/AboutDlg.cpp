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
		_T("------------------------------ʹ��˵��------------------------------\r\n")
		_T("\r\n-���ļ�\r\n")
		_T("---ֱ���϶�����������Ƶ�ļ�������\r\n")
		_T("---ͨ���˵����ļ�����һ�δ򿪵�������\r\n")
		_T("---ͨ���˵����ļ���ָ��Ҫ�������ļ��У�����Ὣ�估���ļ����ڵ���Ƶ��������\r\n")
		_T("---��[��ǰ��Ƶ]��������л�Ҫ�鿴����Ƶ����Ϣ\r\n")
		_T("\r\n-������Ϣ\r\n")
		_T("---ͨ��Ctrl+C���Ƶ����а��ٽ��д���\r\n")
		_T("---ͨ���˵��������������浽ָ��TXT���ļ�����\r\n")
		_T("---�ҵ���Ƶ�ļ����Ҽ��ڵ����˵�ѡ[������Ƶ��Ϣ�����а�]���Ƶ����а�\r\n")
		_T("---ѡ[������Ƶ��Ϣ��TXT]�򱣴浽ָ����TXT�ļ�\r\n")
		_T("\r\n-�����˵�[�Զ�����������]����������Ƶ��Ϣ������ʾָ��������\r\n")
		_T("\r\n-ע����Ƴ��Ҽ������ԱȨ�ޣ��������Win7�ҿ�����UAC�������ù���ԱȨ�����У�������ע����Ƴ�\r\n")		
		_T("\r\nMD5ֵ�Ǹ����ļ���С����ó�\r\n")
		_T("\r\n\t\t\t\t Just Fancy ( 551907703 )\r\n")
		_T("\t\t\t\t\tAug. 19, 2010")
		;

	SetItemText(IDC_EDIT1, szInfo);
	//SendDlgItemMessage(_hWnd, IDC_EDIT1, EM_SETSEL, 0, 0);
}