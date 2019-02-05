// DlgGuassiannoise.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "yuhuachang.h"
#include "DlgGuassiannoise.h"
#include "afxdialogex.h"


// CDlgGuassiannoise �Ի���

IMPLEMENT_DYNAMIC(CDlgGuassiannoise, CDialog)

CDlgGuassiannoise::CDlgGuassiannoise(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_GuassianNoise, pParent)
	, mean_value(0)
	, variance(0)
{

}

CDlgGuassiannoise::~CDlgGuassiannoise()
{
}

void CDlgGuassiannoise::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Mean, mean_value);
	DDX_Text(pDX, IDC_Variance, variance);
	DDV_MinMaxFloat(pDX, variance, 0, 100000);
}


BEGIN_MESSAGE_MAP(CDlgGuassiannoise, CDialog)
END_MESSAGE_MAP()


// CDlgGuassiannoise ��Ϣ�������
