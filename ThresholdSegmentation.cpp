#include "stdafx.h"
#include "yuhuachang.h"
#include "ThresholdSegmentation.h"
#include "afxdialogex.h"


// CThresholdSegmentation 对话框

IMPLEMENT_DYNAMIC(CThresholdSegmentation, CDialog)

CThresholdSegmentation::CThresholdSegmentation(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PointThre, pParent)
	, val_threshold(0)
{

}

CThresholdSegmentation::~CThresholdSegmentation()
{
}

void CThresholdSegmentation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Thre, val_threshold);
	DDV_MinMaxInt(pDX, val_threshold, 0, 255);
}


BEGIN_MESSAGE_MAP(CThresholdSegmentation, CDialog)
END_MESSAGE_MAP()


// CThresholdSegmentation 消息处理程序
