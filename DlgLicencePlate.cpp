// DlgLicencePlate.cpp : 实现文件
//

#include "stdafx.h"
#include "yuhuachang.h"
#include "DlgLicencePlate.h"
#include "afxdialogex.h"

#define WIDTHBYTES(bits) (((bits)+31)/32*4)

// CDlgLicencePlate 对话框

IMPLEMENT_DYNAMIC(CDlgLicencePlate, CDialog)

CDlgLicencePlate::CDlgLicencePlate(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DlgLicensePlate, pParent)
{
	pStc = NULL;
	pDC = NULL;
}

CDlgLicencePlate::~CDlgLicencePlate()
{
}

void CDlgLicencePlate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLicencePlate, CDialog)
	ON_BN_CLICKED(IDC_LinsenseSeg, &CDlgLicencePlate::OnBnClickedLinsenseseg)
END_MESSAGE_MAP()


// CDlgLicencePlate 消息处理程序


BOOL CDlgLicencePlate::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_Picture)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);*/

	pStc = (CStatic *)GetDlgItem(IDC_Picture);//IDC_VIEW为Picture控件ID  
	pStc->GetClientRect(&rect);//将CWind类客户区的坐标点传给矩形  
	pDC = pStc->GetDC(); //得到Picture控件设备上下文  
	hDC = pDC->GetSafeHdc(); //得到控件设备上下文的句柄

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

/*void CDlgLicencePlate::showPlate(Mat pic)
{
	//imshow("view", pic);
	//DrawcvMat(pic, IDC_Picture);
}*/

void CDlgLicencePlate::DrawcvMat(Mat m_cvImg, UINT ID)//显示mat在picture控件中 
{
	Mat img;
	CRect rect;

	GetDlgItem(ID)->GetClientRect(&rect);   //获取窗体大小
	Rect dst(rect.left, rect.top, rect.right, rect.bottom);
	resize(m_cvImg, img, Size(rect.Width(), rect.Height()));
	unsigned int m_buffer[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
	BITMAPINFO* m_bmi = (BITMAPINFO*)m_buffer; BITMAPINFOHEADER* m_bmih = &(m_bmi->bmiHeader);
	memset(m_bmih, 0, sizeof(*m_bmih)); m_bmih->biSize = sizeof(BITMAPINFOHEADER);
	m_bmih->biWidth = img.cols;
	m_bmih->biHeight = -img.rows; // 在自下而上的位图中 高度为负 
	m_bmih->biPlanes = 1;
	m_bmih->biCompression = BI_RGB;
	m_bmih->biBitCount = 8 * img.channels();
	CDC *pDC = GetDlgItem(ID)->GetDC();
	::StretchDIBits(pDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, rect.Width(), rect.Height(), img.data, (BITMAPINFO*)m_bmi, DIB_RGB_COLORS, SRCCOPY); ReleaseDC(pDC);
}

void  CDlgLicencePlate::drawpic(IplImage * img, unsigned int id)
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

	g_hMemDC = ::CreateCompatibleDC(pDC->m_hDC);//创建兼容DC  

	BYTE bmibuf[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)];
	memset(bmibuf, 0, sizeof(bmibuf));
	BITMAPINFO *pbmi = (BITMAPINFO*)bmibuf;

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = img->width;
	pbmi->bmiHeader.biHeight = img->height;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = 24;
	pbmi->bmiHeader.biCompression = BI_RGB;

	g_hBmp = ::CreateDIBSection(g_hMemDC, pbmi, DIB_RGB_COLORS, (void**)&g_pBits, 0, 0);//创建应用程序可以直接写入的、与设备无关的位图（DIB）  
	g_hOldBmp = (HBITMAP)::SelectObject(g_hMemDC, g_hBmp);//复原兼容DC数据  
	BitBlt(g_hMemDC, 0, 0, width, height, pDC->m_hDC, 0, 0, SRCCOPY);

	//修改图像内容:g_pBits  
	int l_width = WIDTHBYTES(img->width* pbmi->bmiHeader.biBitCount);
	for (int row = 0; row < img->height; row++)
		memcpy(&g_pBits[row*l_width], &img->imageData[(img->height - row - 1)*l_width], l_width);

	TransparentBlt(pDC->m_hDC, 0, 0, width, height, g_hMemDC, 0, 0, img->width, img->height, RGB(0, 0, 0));
	SelectObject(g_hMemDC, g_hOldBmp);

	//释放内存资源  
	ReleaseDC(pDC);
	DeleteDC(g_hMemDC);
	DeleteObject(pic);
	DeleteObject(g_hBmp);
	DeleteObject(g_hOldBmp);
}

void CDlgLicencePlate::OnBnClickedLinsenseseg()
{

	CFileDialog dlg(TRUE, _T("*"), _T("*.*"), OFN_HIDEREADONLY, _T("*.*"));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	CString m_path = dlg.GetPathName();
	int len = m_path.GetLength();
	char* filename = (char*)malloc(len); //动态申请char*大小的空间
	for (int i = 0; i <len; i++)
	{
		filename[i] = m_path[i];
	}
	filename[len] = '\0';
	String myfilename = filename;
	/*linsenceSeg ls;
	Mat img = ls.license_detect(myfilename);
	IplImage tmp = img;
	IplImage* showpic = cvCreateImage(cvSize(img.size[0], img.size[1]), 8, 3);
	cvResize(&tmp, showpic);
	drawpic(showpic, IDC_Picture);
	cvReleaseImage(&showpic);*/
	//DrawcvMat(img, IDC_Picture);
}
