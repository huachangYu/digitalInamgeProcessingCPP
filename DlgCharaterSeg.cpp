// DlgCharaterSeg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "yuhuachang.h"
#include "DlgCharaterSeg.h"
#include "afxdialogex.h"

#define WIDTHBYTES(bits) (((bits)+31)/32*4)

// CDlgCharaterSeg �Ի���

IMPLEMENT_DYNAMIC(CDlgCharaterSeg, CDialog)

CDlgCharaterSeg::CDlgCharaterSeg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CharacterSeg, pParent)
{
	pStc = NULL;
	pDC = NULL;
}

CDlgCharaterSeg::~CDlgCharaterSeg()
{
}

void CDlgCharaterSeg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCharaterSeg, CDialog)
	ON_BN_CLICKED(IDC_B_ShowImg, &CDlgCharaterSeg::OnBnClickedBShowimg)
	ON_BN_CLICKED(IDC_B_CharaterSeg, &CDlgCharaterSeg::OnBnClickedBCharaterseg)
END_MESSAGE_MAP()


// CDlgCharaterSeg ��Ϣ�������


BOOL CDlgCharaterSeg::OnInitDialog()
{
	CDialog::OnInitDialog();

	pStc = (CStatic *)GetDlgItem(IDC_S_Showimg);//IDC_VIEWΪPicture�ؼ�ID  
	pStc->GetClientRect(&rect);//��CWind��ͻ���������㴫������  
	pDC = pStc->GetDC(); //�õ�Picture�ؼ��豸������  
	hDC = pDC->GetSafeHdc(); //�õ��ؼ��豸�����ĵľ��   

	return TRUE;   // return TRUE unless you set the focus to a control
				   // �쳣: OCX ����ҳӦ���� FALSE
}

void  CDlgCharaterSeg::drawpic(IplImage * img, unsigned int id)
{
	BYTE *g_pBits;
	HDC g_hMemDC;
	HBITMAP g_hBmp, g_hOldBmp;
	CDC *pDC;
	CStatic *pic;
	int width, height;
	CRect rect;

	pDC = GetDlgItem(id)->GetDC();
	pic = (CStatic*)GetDlgItem(id);
	pic->GetClientRect(&rect);
	width = rect.Width();
	height = rect.Height();

	g_hMemDC = ::CreateCompatibleDC(pDC->m_hDC);//��������DC  

	BYTE bmibuf[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)];
	memset(bmibuf, 0, sizeof(bmibuf));
	BITMAPINFO *pbmi = (BITMAPINFO*)bmibuf;

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = img->width;
	pbmi->bmiHeader.biHeight = img->height;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = 24;
	pbmi->bmiHeader.biCompression = BI_RGB;

	g_hBmp = ::CreateDIBSection(g_hMemDC, pbmi, DIB_RGB_COLORS, (void**)&g_pBits, 0, 0);//����Ӧ�ó������ֱ��д��ġ����豸�޹ص�λͼ��DIB��  
	g_hOldBmp = (HBITMAP)::SelectObject(g_hMemDC, g_hBmp);//��ԭ����DC����  
	BitBlt(g_hMemDC, 0, 0, width, height, pDC->m_hDC, 0, 0, SRCCOPY);

	//�޸�ͼ������:g_pBits  
	int l_width = WIDTHBYTES(img->width* pbmi->bmiHeader.biBitCount);
	for (int row = 0; row < img->height; row++)
		memcpy(&g_pBits[row*l_width], &img->imageData[(img->height - row - 1)*l_width], l_width);

	TransparentBlt(pDC->m_hDC, 0, 0, width, height, g_hMemDC, 0, 0, img->width, img->height, RGB(0, 0, 0));
	SelectObject(g_hMemDC, g_hOldBmp);

	//�ͷ��ڴ���Դ  
	ReleaseDC(pDC);
	DeleteDC(g_hMemDC);
	DeleteObject(pic);
	DeleteObject(g_hBmp);
	DeleteObject(g_hOldBmp);
}



void CDlgCharaterSeg::OnBnClickedBShowimg()
{
	CFileDialog dlg(TRUE, _T("*"), _T("*.*"), OFN_HIDEREADONLY, _T("*.*"));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	CString m_path = dlg.GetPathName();
	int len = m_path.GetLength();
	char* filename = (char*)malloc(len); //��̬����char*��С�Ŀռ�
	for (int i = 0; i <len; i++)
	{
		filename[i] = m_path[i];
	}
	filename[len] = '\0';
	myfilename = filename;
	IplImage* img = cvLoadImage(filename);
	drawpic(img, IDC_S_Showimg);//IDC_S_Showimg ΪPicture Control �ؼ���ID  
	cvReleaseImage(&img);
}


void CDlgCharaterSeg::OnBnClickedBCharaterseg()
{
	cv::Mat imgSrc = cv::imread(myfilename, 1);
	cv::Mat img_gray;
	cvtColor(imgSrc, img_gray, CV_BGR2GRAY);
	threshold(img_gray, img_gray, 100, 255, CV_THRESH_BINARY_INV); //��ֵ��
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(img_gray, contours, hierarchy,
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //��ȡ�������������������contours�У������ȼ������hierarchy��

	for (int i = 0; i < contours.size(); i++)
	{
		cv::Rect rect = cv::boundingRect(cv::Mat(contours[i]));// ���������Ĵ�ֱ�߽���С���Σ���������ͼ�����±߽�ƽ�е�
		rectangle(imgSrc, rect.tl(), rect.br(), cv::Scalar(255, 0, 0));//������β�����������
	}
	IplImage tmp = imgSrc;
	IplImage* showpic = cvCreateImage(cvSize(imgSrc.size[0], imgSrc.size[1]), 8, 3);
	cvResize(&tmp, showpic);
	drawpic(showpic, IDC_S_Showimg);//IDC_S_Showimg ΪPicture Control �ؼ���ID  
	cvReleaseImage(&showpic);
}