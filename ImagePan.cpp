// ImagePan.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "yuhuachang.h"
#include "ImagePan.h"
#include "afxdialogex.h"


// CImagePan �Ի���

IMPLEMENT_DYNAMIC(CImagePan, CDialog)

CImagePan::CImagePan(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PAN, pParent)
	, pan_x(0)
	, pan_y(0)
{

}

CImagePan::~CImagePan()
{
}

void CImagePan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PAN_X, pan_x);
	DDX_Text(pDX, IDC_EDIT_PAN_Y, pan_y);
}


BEGIN_MESSAGE_MAP(CImagePan, CDialog)
END_MESSAGE_MAP()


// CImagePan ��Ϣ�������
