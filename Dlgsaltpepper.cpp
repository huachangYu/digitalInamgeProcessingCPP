// Dlgsaltpepper.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "yuhuachang.h"
#include "Dlgsaltpepper.h"
#include "afxdialogex.h"


// CDlgsaltpepper �Ի���

IMPLEMENT_DYNAMIC(CDlgsaltpepper, CDialog)

CDlgsaltpepper::CDlgsaltpepper(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DlgSaltPepperNoise, pParent)
	, SNR(0)
{

}

CDlgsaltpepper::~CDlgsaltpepper()
{
}

void CDlgsaltpepper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SNR, SNR);
	DDV_MinMaxFloat(pDX, SNR, 0, 1);
}


BEGIN_MESSAGE_MAP(CDlgsaltpepper, CDialog)
END_MESSAGE_MAP()


// CDlgsaltpepper ��Ϣ�������
