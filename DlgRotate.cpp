// DlgRotate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "yuhuachang.h"
#include "DlgRotate.h"
#include "afxdialogex.h"


// CDlgRotate �Ի���

IMPLEMENT_DYNAMIC(CDlgRotate, CDialog)

CDlgRotate::CDlgRotate(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_Rotate, pParent)
	, angle(0)
{

}

CDlgRotate::~CDlgRotate()
{
}

void CDlgRotate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Angle, angle);
}


BEGIN_MESSAGE_MAP(CDlgRotate, CDialog)
END_MESSAGE_MAP()


// CDlgRotate ��Ϣ�������
