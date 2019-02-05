#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "yuhuachang.h"
#endif

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)

#include "yuhuachangDoc.h"
#include "yuhuachangView.h"
#include "DlgIntensity.h"
#include "ImagePan.h"
#include "ThresholdSegmentation.h"
#include "DlgPointThre.h"
#include "DlgRotate.h"
#include "DlgGuassiannoise.h"
#include "Dlgsaltpepper.h"
#include "DlgCharaterSeg.h"
#include "DlgLicencePlate.h"
#include "imageLicence.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CyuhuachangView, CView)

BEGIN_MESSAGE_MAP(CyuhuachangView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CyuhuachangView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CyuhuachangView::OnFileSave)
	ON_COMMAND(ID_VIEW_INTENSITY, &CyuhuachangView::OnViewIntensity)
	ON_COMMAND(ID_COLOR2GRAY, &CyuhuachangView::OnColor2gray)
	ON_COMMAND(ID_IMAGE_PANNING, &CyuhuachangView::OnImagePanning)
	ON_COMMAND(ID_THRESHOLD, &CyuhuachangView::OnThreshold)
	ON_COMMAND(ID_REVERSE, &CyuhuachangView::OnReverse)
	ON_COMMAND(ID_POINT_TRANS, &CyuhuachangView::OnPointTrans)
	ON_COMMAND(ID_PointThre, &CyuhuachangView::OnPointthre)
	ON_COMMAND(ID_IMAGE_MIRROR, &CyuhuachangView::OnImageMirror)
	ON_COMMAND(ID_Mutiply_1_5, &CyuhuachangView::OnMutiply15)
	ON_COMMAND(ID_Mutiply_0_6, &CyuhuachangView::OnMutiply06)
	ON_COMMAND(ID_NonLinnearTrans_DD_255, &CyuhuachangView::OnNonlinneartransDd255)
	ON_COMMAND(ID_Edge_Sobel, &CyuhuachangView::OnEdgeSobel)
	ON_COMMAND(ID_Edge_Laplace, &CyuhuachangView::OnEdgeLaplace)
	ON_COMMAND(ID_AverageFilter, &CyuhuachangView::OnAveragefilter)
	ON_COMMAND(ID_Edge_Roberts, &CyuhuachangView::OnEdgeRoberts)
	ON_COMMAND(ID_IMAGE_ROTATE, &CyuhuachangView::OnImageRotate)
	ON_COMMAND(ID_HistogramEqualization, &CyuhuachangView::OnHistogramequalization)
	ON_COMMAND(ID_HistogramMatch, &CyuhuachangView::OnHistogrammatch)
	ON_COMMAND(ID_MediumFilter, &CyuhuachangView::OnMediumfilter)
	ON_COMMAND(ID_FormerPic, &CyuhuachangView::OnFormerpic)
	ON_COMMAND(ID_AddNoiseGuassian, &CyuhuachangView::OnAddnoiseguassian)
	ON_COMMAND(ID_AddNoiseSaltPepper, &CyuhuachangView::OnAddnoisesaltpepper)
	ON_COMMAND(ID_Mosaic, &CyuhuachangView::OnMosaic)
	ON_COMMAND(ID_GrayToColorMix, &CyuhuachangView::OnGraytocolormix)
	ON_COMMAND(ID_CharaterSeg, &CyuhuachangView::OnCharaterseg)
	ON_COMMAND(ID_SharpeningLaplace, &CyuhuachangView::OnSharpeninglaplace)
	ON_COMMAND(ID_SharpeningGrad, &CyuhuachangView::OnSharpeninggrad)
	ON_COMMAND(ID_LinsenceSegmentation, &CyuhuachangView::OnLinsencesegmentation)
	ON_COMMAND(ID_LisenseStrSeg, &CyuhuachangView::OnLisensestrseg)
	//ON_COMMAND(ID_32838, &CyuhuachangView::On32838)
END_MESSAGE_MAP()

CyuhuachangView::CyuhuachangView()
{
	m_pBmfh = new BITMAPFILEHEADER;
	nWidth = nHeight = LineBytes = 0;
	if (m_pPixel != NULL)
	{
		m_pPixel = NULL;

	}
	draw_x = 0;
	draw_y = 0;
}

CyuhuachangView::~CyuhuachangView()
{
}

BOOL CyuhuachangView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void CyuhuachangView::set_drawpos(int x, int y)
{
	draw_x = x;
	draw_y = y;
}

void CyuhuachangView::save_threshold_figure(int threshold)
{
	UpdateData();
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	//判断是否是真彩色图像
	bitCount = bmih.biBitCount;//获取位数
	if (bitCount == 24)
	{
		MessageBox(_T("彩色图像，无法反色！"));
		return;
	}
	//判断图像是否经过压缩
	if (bmih.biCompression != BI_RGB)
	{
		MessageBox(_T("不处理压缩过的图像！"));
		return;
	}
	memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpFileHeader.bfOffBits = 54 + sizeof(RGBQUAD) * 256;
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmih.biSizeImage / 3;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfType = 0x4d42;

	bmpInfoHeader.biBitCount = 8;//变成灰度图像是8位
	bmpInfoHeader.biSize = 40;//信息头固定大小为40
	bmpInfoHeader.biHeight = bmih.biHeight;
	bmpInfoHeader.biWidth = bmih.biWidth;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bmih.biSizeImage;
	bmpInfoHeader.biXPelsPerMeter = bmih.biXPelsPerMeter;
	bmpInfoHeader.biYPelsPerMeter = bmih.biYPelsPerMeter;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 256;
	RGBQUAD rgbquad[256];
	for (int i = 0; i<256; i++)
	{
		rgbquad[i].rgbBlue = i;
		rgbquad[i].rgbGreen = i;
		rgbquad[i].rgbRed = i;
		rgbquad[i].rgbReserved = 0;
	}
	//满足长度和宽度都是4的整数倍
	nWidth = (bmpInfoHeader.biWidth / 4) * 4;
	if (nWidth < bmpInfoHeader.biWidth)
	{
		nWidth = nWidth + 4;
	}
	nHeight = (bmpInfoHeader.biHeight / 4) * 4;
	if (nHeight < bmpInfoHeader.biHeight)
	{
		nHeight = nHeight + 4;
	}
	BYTE unit;
	grayImage = new BYTE[nWidth*nHeight];//the gray turned image
	memset(grayImage, 0, nWidth*nHeight);
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			unit = (BYTE)(255 - m_pPixel[i*nWidth + j]);
			if (unit > threshold)
			{
				unit = 0;
			}
			else {
				unit = 255;
			}
			grayImage[i*nWidth + j] = unit;
		}
	}
	CFileDialog dlg(false, _T("*.bmp"), NULL, OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT, _T("BMP files(*bmp)|*.bmp||"));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	s_path = dlg.GetPathName();
	CFile dib(s_path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	dib.Write(&bmpFileHeader, sizeof(BITMAPFILEHEADER));
	dib.Write(&bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	dib.Write((void*)&rgbquad, sizeof(RGBQUAD) * 256);//写入调色板
	dib.Write(grayImage, nHeight*nWidth);
	dib.Close();

	delete[]grayImage;

}

void CyuhuachangView::OnDraw(CDC* pDC)
{
	CyuhuachangDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (m_path == "")
	{
		return;
	}

	BITMAPINFOHEADER bmih;
	memcpy(&bmih, m_pBmInfo, sizeof(BITMAPINFOHEADER));
	long width = bmih.biWidth;
	long height = bmih.biHeight;
	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	::StretchDIBits(pDC->GetSafeHdc(), draw_x, draw_y, width, height, 0, 0, width, height, m_pPixel,
		(BITMAPINFO*)m_pBmInfo, DIB_RGB_COLORS, SRCCOPY);
}

// CyuhuachangView 打印

BOOL CyuhuachangView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CyuhuachangView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CyuhuachangView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

#ifdef _DEBUG
void CyuhuachangView::AssertValid() const
{
	CView::AssertValid();
}

void CyuhuachangView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CyuhuachangDoc* CyuhuachangView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CyuhuachangDoc)));
	return (CyuhuachangDoc*)m_pDocument;
}
#endif //_DEBUG

void CyuhuachangView::OnFileOpen()
{
	CFileDialog dlg(TRUE, _T("bmp"), _T(".bmp"), OFN_HIDEREADONLY, _T("BMP Files(*.bmp)|*.bmp||"));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	m_path = dlg.GetPathName();
	CFile dib(m_path, CFile::modeRead);
	dib.Read(m_pBmfh, sizeof(BITMAPFILEHEADER));
	m_pBmInfo = new BYTE[m_pBmfh->bfOffBits - 14]; 
	dib.Read(m_pBmInfo, m_pBmfh->bfOffBits - 14);
	memcpy(&bmih, m_pBmInfo, sizeof(BITMAPINFOHEADER));
	nWidth = bmih.biWidth;
	bitCount = bmih.biBitCount;
	nHeight = bmih.biHeight;
	LineBytes = (nWidth*bitCount + 31) / 32 * 4; 
	m_pPixel = new BYTE[nHeight*LineBytes];
	dib.Read(m_pPixel, nHeight*LineBytes);
	dib.Close();
	Invalidate(TRUE);
}

void CyuhuachangView::OnFileSave()
{
	CFileDialog dlg(false, _T("*.bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("BMP files(*.bmp)|*.bmp||"));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	CFile dib(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	dib.Write(m_pBmfh, sizeof(BITMAPFILEHEADER));
	dib.Write(m_pBmInfo, m_pBmfh->bfOffBits - 14);
	memcpy(&bmih, m_pBmInfo, sizeof(BITMAPINFOHEADER));
	nWidth = bmih.biWidth;
	bitCount = bmih.biBitCount;
	nHeight = bmih.biHeight;
	LineBytes = (nWidth*bitCount + 31) / 32 * 4;
	dib.Write(m_pPixel, nHeight*LineBytes);
	dib.Close();
}

void CyuhuachangView::OnViewIntensity()
{
	UpdateData();  //更新变量数据
				   //判断是否数据读入成功
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	//判断是否是真彩色图像
	bitCount = bmih.biBitCount;//获取位数
	if (bitCount != 8)
	{
		MessageBox(_T("不是灰度图像，请灰度化后查看!"));
		return;
	}
	// 创建对话框
	CDlgIntensity dlgPara;
	dlgPara.m_pPixel = m_pPixel;
	dlgPara.m_nWidth = nWidth;
	dlgPara.m_lHeight = nHeight;
	dlgPara.m_iLowGray = 0;
	dlgPara.m_iUpGray = 255;
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		return;
	}
}

void CyuhuachangView::OnColor2gray()
{
	UpdateData();
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	//判断是否是真彩色图像
	bitCount = bmih.biBitCount;//获取位数
	if (bitCount != 24)
	{
		MessageBox(_T("不是真彩色图像，不需要灰度化！"));
		return;
	}
	//判断图像是否经过压缩
	if (bmih.biCompression != BI_RGB)
	{
		MessageBox(_T("不处理压缩过的图像！"));
		return;
	}
	//构造灰度图像文件头
	memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpFileHeader.bfOffBits = 54 + sizeof(RGBQUAD) * 256;
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmih.biSizeImage / 3;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfType = 0x4d42;
								  
	bmpInfoHeader.biBitCount = 8;//变成灰度图像是8位
	bmpInfoHeader.biSize = 40;//信息头固定大小为40
	bmpInfoHeader.biHeight = bmih.biHeight;
	bmpInfoHeader.biWidth = bmih.biWidth;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bmih.biSizeImage / 3;
	bmpInfoHeader.biXPelsPerMeter = bmih.biXPelsPerMeter;
	bmpInfoHeader.biYPelsPerMeter = bmih.biYPelsPerMeter;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 256;

	//构造灰度图像的设色板：
	RGBQUAD rgbquad[256];
	for (int i = 0; i<256; i++)
	{
		rgbquad[i].rgbBlue = i;
		rgbquad[i].rgbGreen = i;
		rgbquad[i].rgbRed = i;
		rgbquad[i].rgbReserved = 0;
	}
	//满足长度和宽度都是4的整数倍
	nWidth = (bmpInfoHeader.biWidth / 4) * 4;
	if (nWidth < bmpInfoHeader.biWidth)
	{
		nWidth = nWidth + 4;
	}
	nHeight = (bmpInfoHeader.biHeight / 4) * 4;
	if (nHeight < bmpInfoHeader.biHeight)
	{
		nHeight = nHeight + 4;
	}
	//将RGB转换为灰度值
	int red, green, blue;
	BYTE gray;
	grayImage = new BYTE[nWidth*nHeight];//the gray turned image
	memset(grayImage, 0, nWidth*nHeight);
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			red = m_pPixel[3 * (i*nWidth + j)];
			green = m_pPixel[3 * (i*nWidth + j) + 1];
			blue = m_pPixel[3 * (i*nWidth + j) + 2];
			gray = (BYTE)((77 * red + 151 * green + 28 * blue) >> 8);
			grayImage[i*nWidth + j] = gray;
		}
	}
	//保存为灰度图像
	CFileDialog dlg(false, _T("*.bmp"), NULL, OFN_HIDEREADONLY | 
		OFN_OVERWRITEPROMPT, _T("BMP files(*bmp)|*.bmp||"));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	s_path = dlg.GetPathName();
	CFile dib(s_path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	dib.Write(&bmpFileHeader, sizeof(BITMAPFILEHEADER));
	dib.Write(&bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	dib.Write((void*)&rgbquad, sizeof(RGBQUAD) * 256);//写入调色板
	dib.Write(grayImage, nHeight*nWidth);
	dib.Close();

	delete[]grayImage;
}

void CyuhuachangView::OnImagePanning()
{
	CImagePan dlg;
	if (dlg.DoModal() == IDOK)
	{
		draw_x+=dlg.pan_x;
		draw_y += dlg.pan_y;
		Invalidate(TRUE);
	}
}

void CyuhuachangView::OnThreshold()
{
	CThresholdSegmentation cs;
	if (cs.DoModal() == IDOK)
	{
		save_threshold_figure(cs.val_threshold);
	}
}

void CyuhuachangView::OnReverse()
{
	UpdateData();
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	//判断是否是真彩色图像
	bitCount = bmih.biBitCount;//获取位数
	if (bitCount == 24)
	{
		MessageBox(_T("是真彩色图像，无法反色！"));
		return;
	}
	//判断图像是否经过压缩
	if (bmih.biCompression != BI_RGB)
	{
		MessageBox(_T("不处理压缩过的图像！"));
		return;
	}
	//构造灰度图像文件头
	memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpFileHeader.bfOffBits = 54 + sizeof(RGBQUAD) * 256;
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmih.biSizeImage / 3;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfType = 0x4d42;

	bmpInfoHeader.biBitCount = 8;//变成灰度图像是8位
	bmpInfoHeader.biSize = 40;//信息头固定大小为40
	bmpInfoHeader.biHeight = bmih.biHeight;
	bmpInfoHeader.biWidth = bmih.biWidth;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bmih.biSizeImage;
	bmpInfoHeader.biXPelsPerMeter = bmih.biXPelsPerMeter;
	bmpInfoHeader.biYPelsPerMeter = bmih.biYPelsPerMeter;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 256;

	//构造灰度图像的设色板：
	RGBQUAD rgbquad[256];
	for (int i = 0; i<256; i++)
	{
		rgbquad[i].rgbBlue = i;
		rgbquad[i].rgbGreen = i;
		rgbquad[i].rgbRed = i;
		rgbquad[i].rgbReserved = 0;
	}
	//满足长度和宽度都是4的整数倍
	nWidth = (bmpInfoHeader.biWidth / 4) * 4;
	if (nWidth < bmpInfoHeader.biWidth)
	{
		nWidth = nWidth + 4;
	}
	nHeight = (bmpInfoHeader.biHeight / 4) * 4;
	if (nHeight < bmpInfoHeader.biHeight)
	{
		nHeight = nHeight + 4;
	}
	//将RGB转换为灰度值
	BYTE gray;
	grayImage = new BYTE[nWidth*nHeight];
	memset(grayImage, 0, nWidth*nHeight);
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			gray = (BYTE)(255-m_pPixel[i*nWidth + j]);
			grayImage[i*nWidth + j] = gray;
		}
	}
	//保存为灰度图像
	CFileDialog dlg(false, _T("*.bmp"), NULL, OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT, _T("BMP files(*bmp)|*.bmp||"));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	s_path = dlg.GetPathName();
	CFile dib(s_path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	dib.Write(&bmpFileHeader, sizeof(BITMAPFILEHEADER));
	dib.Write(&bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	dib.Write((void*)&rgbquad, sizeof(RGBQUAD) * 256);//写入调色板
	dib.Write(grayImage, nHeight*nWidth);
	dib.Close();

	delete[]grayImage;
}

void CyuhuachangView::OnPointTrans()
{
	UpdateData();//更新变量数据
	BeginWaitCursor();//更改光标形状

	nWidth = bmih.biWidth;//获取宽度
	bitCount = bmih.biBitCount;//获取位数
	nHeight = bmih.biHeight;//获取高度
	long nLineBytes = (nWidth*bitCount + 31) / 32 * 4;//计算每行像素所占的字节数

	int i, j;
	for (i = 0; i < nWidth; i++)
	{
		for (j = 0; j < nHeight; j++)
		{
			m_pPixel[j*nWidth + i] = 255 - m_pPixel[j*nWidth + i];
		}
	}
	EndWaitCursor();
	Invalidate(TRUE);
}

void CyuhuachangView::OnPointthre()
{
	UpdateData();
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	bitCount = bmih.biBitCount;
	if (bitCount != 8)
	{
		MessageBox(_T("不是灰度图像，请灰度化后查看！"));
		return;
	}
	CDlgPointThre dlgPara;
	BYTE bThre;
	dlgPara.m_bThre=128;
	if (dlgPara.DoModal() != IDOK)
	{
		return;
	}
	bThre = dlgPara.m_bThre;
	delete dlgPara;
	BeginWaitCursor();
	int i, j;
	for (i = 0; i < nWidth; i++)
	{
		for (j = 0; j < nHeight; j++)
		{
			if (m_pPixel[j*nWidth + i] > bThre)
			{
				m_pPixel[j*nWidth + i] = 255;
			}
			else {
				m_pPixel[j*nWidth + i] = 0;
			}
		}
	}
	EndWaitCursor();
	Invalidate(TRUE);
}

void CyuhuachangView::OnImageMirror()
{
	setGrayimage();
	BYTE gray;
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			gray = (BYTE)m_pPixel[i*nWidth + (nWidth-j-1)];
			grayImage[i*nWidth + j] = gray;
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnMutiply15()
{
	gray_linear_trans(1.5, 0);
}

void CyuhuachangView::OnMutiply06()
{
	gray_linear_trans(0.6, 0);
}

void CyuhuachangView::gray_linear_trans(double a, double b)
{
	UpdateData();
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	bitCount = bmih.biBitCount;
	if (bitCount != 8)
	{
		MessageBox(_T("不是灰度图像，请灰度化后查看！"));
		return;
	}
	int i, j;
	BYTE tmp;
	for (i = 0; i < nWidth; i++)
	{
		for (j = 0; j < nHeight; j++)
		{
			tmp = BYTE(a*m_pPixel[j*nWidth + i]+b);
			if (tmp > 255)
			{
				m_pPixel[j*nWidth + i] = 255;
			}
			else if (tmp < 0)
			{
				m_pPixel[j*nWidth + i] = 0;
			}
			else {
				m_pPixel[j*nWidth + i] = tmp;
			}
		}
	}
	Invalidate(TRUE);
}

void CyuhuachangView::image_rotate(double ang)//有问题
{
	setGrayimage();
	float d1;       
	float d2;
	float fCosa = cos(ang), fSina = sin(ang);
	int i1, j1;
	d1 = (float)(-0.5 * (nWidth - 1) * fCosa - 0.5 * (nHeight - 1) * fSina// 将经常用到的两个常数事先求出，以便作为常数使用
		+ 0.5 * (nWidth - 1));
	d2 = (float)(0.5 * (nWidth - 1) * fSina - 0.5 * (nHeight - 1) * fCosa
		+ 0.5 * (nHeight - 1));
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			//(nHeight - 1 - i)
			i1 = int(-((float)j) * fSina + ((float)(nHeight - 1 - i)) * fCosa + d2 + 0.5);// 计算该像素在源DIB中的坐标
			j1 = int(((float)j) * fCosa + ((float)(nHeight - 1 - i)) * fSina + d1 + 0.5);
			if ((j1 >= 0) && (j1< nWidth) && (i1 >= 0) && (i1< nHeight))
			{
				grayImage[(nHeight - 1 - i)*nWidth + j] = m_pPixel[i1*nWidth + j1];
			}
			else
			{
				grayImage[(nHeight - 1 - i)*nWidth + j] = 255;// 源图中不存在的像素，赋为255
			}
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnNonlinneartransDd255()
{
	UpdateData();
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	bitCount = bmih.biBitCount;
	if (bitCount != 8)
	{
		MessageBox(_T("不是灰度图像，请灰度化后查看！"));
		return;
	}
	int i, j;
	BYTE tmp;
	for (i = 0; i < nWidth; i++)
	{
		for (j = 0; j < nHeight; j++)
		{
			tmp = BYTE(m_pPixel[j*nWidth + i] *m_pPixel[j*nWidth + i]/255);
			if (tmp > 255)
			{
				m_pPixel[j*nWidth + i] = 255;
			}
			else if (tmp < 0)
			{
				m_pPixel[j*nWidth + i] = 0;
			}
			else {
				m_pPixel[j*nWidth + i] = tmp;
			}
		}
	}
	Invalidate(TRUE);
}

void CyuhuachangView::OnEdgeSobel()
{
	setGrayimage();
	BYTE imgDataH, imgDataV;
	int gradEdge = 0;
	int T = 1;
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			if ((j != 0) && (j != nHeight) && (i != 0) && (i != nHeight))
			{
				imgDataH = (m_pPixel[(i + 1)*nWidth + j - 1] + 2 * m_pPixel[(i + 1)*nWidth + j] +
					m_pPixel[(i + 1)*nWidth + j + 1] - m_pPixel[(i - 1)*nWidth + j - 1] -
					2 * m_pPixel[(i - 1)*nWidth + j] - m_pPixel[(i - 1)*nWidth + j + 1]) / 4;
				imgDataV = (m_pPixel[(i - 1)*nWidth + j + 1] + 2 * m_pPixel[i*nWidth + j + 1] +
					m_pPixel[(i + 1)*nWidth + j + 1] - m_pPixel[(i - 1)*nWidth + j - 1] -
					2 * m_pPixel[i*nWidth + j - 1] - m_pPixel[(i + 1)*nWidth + j - 1]) / 4;
			}
			else
			{
				imgDataH = 0;
				imgDataV = 0;
			}
			grayImage[i*nWidth + j] = BYTE(abs(imgDataH) + abs(imgDataV));
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnEdgeLaplace()
{
	setGrayimage();
	BYTE p; 
	//const int T = 255;
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			if ((j != 0) && (j != nHeight) && (i != 0) && (i != nHeight))
			{
				p = (BYTE)(8 * m_pPixel[nWidth*i + j] - 1 * m_pPixel[nWidth*(i - 1) + j] - 1 * m_pPixel[nWidth*(i + 1) + j]
					- 1 * m_pPixel[nWidth*(i - 1) + j - 1] - 1 * m_pPixel[nWidth*i + j - 1] - 1 * m_pPixel[nWidth*(i + 1) + j - 1]
					- 1 * m_pPixel[nWidth*(i - 1) + j + 1] - 1 * m_pPixel[nWidth*i + j + 1] - 1 * m_pPixel[nWidth*(i + 1) + j + 1]);
			}
			else
			{
				p = 0;
			}
			/*if (p > T)
			{
				p = p;
			}
			else {
				p = 0;
			}*/
			grayImage[i*nWidth + j] = (BYTE)p;
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnAveragefilter()
{
	setGrayimage();
	BYTE p;
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			if ((j != 0) && (j != nWidth) && (i != 0) && (i != nHeight))
			{
				/*p = (BYTE)((float)(m_pPixel[(i - 1)*nWidth + (j - 1)]
					+ m_pPixel[(i - 1)*nWidth + j] + m_pPixel[(i - 1)*nWidth + (j + 1)]
					+ m_pPixel[i*nWidth + (j - 1)] + m_pPixel[i*nWidth + j] + m_pPixel[i*nWidth + j + 1]
					+ m_pPixel[(i + 1)*nWidth + (j - 1)] + m_pPixel[(i + 1)*nWidth + j]
					+ m_pPixel[(i + 1)*nWidth + j + 1]) / 9 + 0.5);*/
				p = (BYTE)((float)(m_pPixel[(i - 1)*nWidth + (j - 1)]
					+ m_pPixel[(i - 1)*nWidth + j] + m_pPixel[(i - 1)*nWidth + (j + 1)]
					+ m_pPixel[i*nWidth + (j - 1)]  + m_pPixel[i*nWidth + j + 1]
					+ m_pPixel[(i + 1)*nWidth + (j - 1)] + m_pPixel[(i + 1)*nWidth + j]
					+ m_pPixel[(i + 1)*nWidth + j + 1]) / 8 + 0.5);
			}
			else
			{
				p = grayImage[i*nWidth + j];
			}
			grayImage[i*nWidth + j] = (BYTE)p;
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnEdgeRoberts()
{
	setGrayimage();
	BYTE p;
	BYTE pixel[4];
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			if ((j != 0) && (j != nWidth) && (i != 0) && (i != nHeight))
			{
				//使用了2*2的模板
				pixel[0] = m_pPixel[i * nWidth + j];
				pixel[1] = m_pPixel[i * nWidth + j + 1];
				pixel[2] = m_pPixel[(i + 1) * nWidth + j];
				pixel[3] = m_pPixel[(i + 1) * nWidth + j + 1];

				/*p = sqrt(double((pixel[0] - pixel[3]) * (pixel[0] - pixel[3])
					+ (pixel[1] - pixel[2]) * (pixel[1] - pixel[2])));*/
				p = abs(pixel[0] - pixel[3])+abs(pixel[1] - pixel[2]);
			}
			else
			{
				p = grayImage[i*nWidth + j];
			}
			grayImage[i*nWidth + j] = (BYTE)p;
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnImageRotate()
{
	CDlgRotate dlg;
	if (dlg.DoModal() == IDOK)
	{
		image_rotate(dlg.angle);
	}
}

void CyuhuachangView::Histogram_Statistic(int *gray, float *probability, int lWidth, int lHeight)
{
	long i, j;	
	BYTE pixel; //当前像素值
	for (j = 0; j <lHeight; j++) //逐个扫面图像中的像素点,进行灰度计数统计
	{
		for (i = 0; i <lWidth; i++)
		{
			pixel = m_pPixel[lWidth * j + i];
			gray[pixel]++;// 灰度统计计数
		}
	}
	for (i = 0; i<256; i++)// 计算灰度概率密度
	{
		probability[i] = gray[i] / (lHeight * lWidth *1.0f);
	}
}

void CyuhuachangView::setGrayimage()
{
	UpdateData();
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	//判断是否是真彩色图像
	bitCount = bmih.biBitCount;//获取位数
	if (bitCount == 24)
	{
		MessageBox(_T("是真彩色图像，无法处理！"));
		return;
	}
	//判断图像是否经过压缩
	if (bmih.biCompression != BI_RGB)
	{
		MessageBox(_T("不处理压缩过的图像！"));
		return;
	}
	//构造灰度图像文件头
	memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpFileHeader.bfOffBits = 54 + sizeof(RGBQUAD) * 256;
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmih.biSizeImage / 3;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfType = 0x4d42;

	bmpInfoHeader.biBitCount = 8;//变成灰度图像是8位
	bmpInfoHeader.biSize = 40;//信息头固定大小为40
	bmpInfoHeader.biHeight = bmih.biHeight;
	bmpInfoHeader.biWidth = bmih.biWidth;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bmih.biSizeImage;
	bmpInfoHeader.biXPelsPerMeter = bmih.biXPelsPerMeter;
	bmpInfoHeader.biYPelsPerMeter = bmih.biYPelsPerMeter;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 256;

	//构造灰度图像的设色板：
	RGBQUAD rgbquad[256];
	for (int i = 0; i<256; i++)
	{
		rgbquad[i].rgbBlue = i;
		rgbquad[i].rgbGreen = i;
		rgbquad[i].rgbRed = i;
		rgbquad[i].rgbReserved = 0;
	}
	//满足长度和宽度都是4的整数倍
	nWidth = (bmpInfoHeader.biWidth / 4) * 4;
	if (nWidth < bmpInfoHeader.biWidth)
	{
		nWidth = nWidth + 4;
	}
	nHeight = (bmpInfoHeader.biHeight / 4) * 4;
	if (nHeight < bmpInfoHeader.biHeight)
	{
		nHeight = nHeight + 4;
	}
	grayImage = new BYTE[nWidth*nHeight];
	memset(grayImage, 0, nWidth*nHeight);
}

void CyuhuachangView::OnHistogramequalization()
{
	setGrayimage(); 
	float fPro[256]; // 原图像灰度分布概率密度变量
	int gray[256];
	BYTE pixel;
	memset(fPro, 0, sizeof(fPro));
	memset(gray, 0, sizeof(gray));
	float temp[256];
	int nRst[256];
	memset(temp, 0, sizeof(temp));
	Histogram_Statistic(gray, fPro, nWidth, nHeight);
	for (int i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			temp[0] = fPro[0];
		}
		else
		{
			temp[i] = temp[i - 1] + fPro[i];
		}
		nRst[i] = (int)(255.0f * temp[i] + 0.5f);
	}
	for (int i = 0; i <nHeight; i++)
	{
		for (int j = 0; j <nWidth; j++)
		{
			pixel = m_pPixel[i*nWidth+j];
			grayImage[i*nWidth + j] = (BYTE)(nRst[pixel]);
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnHistogrammatch()
{
	setGrayimage();
	float fPro[256];// 原图像灰度分布概率密度变量
	int gray[256];
	BYTE pixel;
	memset(fPro, 0, sizeof(fPro));
	memset(gray, 0, sizeof(gray));
	float temp[256];
	int nMap[256];

	//设置默认参数,要修改啊，大兄弟！！！！用界面的方式输入
	const int GRAY_LEVEL = 64;
	BYTE bGray = GRAY_LEVEL;
	int npMap[GRAY_LEVEL];//规定直方图映射关系,这里规定直方图的灰度级为64
	float fpPro[GRAY_LEVEL];//规定灰度分布概率
	float b = (float)GRAY_LEVEL*((float) GRAY_LEVEL - 1) / 2;
	float a = 1.0f / b;
	for (int i = 0; i<GRAY_LEVEL; i++)
	{
		npMap[i] = i * 4;
		fpPro[i] = a*i;
	}
	memset(temp, 0, sizeof(temp));
	Histogram_Statistic(gray, fPro, nWidth, nHeight);
	for (int i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			temp[0] = fPro[0];
		}
		else
		{
			temp[i] = temp[i - 1] + fPro[i];
		}
		fPro[i] = temp[i];
	}
	for (int i = 0; i < bGray; i++)// 计算规定变换后的累计直方图
	{
		if (i == 0)
		{
			temp[0] = fpPro[0];
		}
		else
		{
			temp[i] = temp[i - 1] + fpPro[i];
		}
		fpPro[i] = temp[i];
	}
	for (int i = 0; i < 256; i++)// 确定映射关系
	{
		int m = 0; // 最接近的规定直方图灰度级
		float min_value = 1.0f; // 最小差值
		for (int j = 0; j < bGray; j++) // 枚举规定直方图各灰度
		{	
			float now_value = 0.0f; // 当前差值
			if (fPro[i] - fpPro[j] >= 0.0f) // 差值计算
			{
				now_value = fPro[i] - fpPro[j];
			}
			else
			{
				now_value = fpPro[j] - fPro[i]; // 寻找最接近的规定直方图灰度级
			}
			if (now_value < min_value)
			{
				m = j;// 最接近的灰度级
				min_value = now_value; // 最小差值
			}
		}
		nMap[i] = npMap[m];// 建立灰度映射表
	}
	for (int j = 0; j < nHeight; j++)
	{
		for (int i = 0; i < nWidth; i++)
		{
			pixel = m_pPixel[i*nWidth + j];
			grayImage[i*nWidth + j] = (BYTE)(nMap[pixel]);
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnMediumfilter()
{
	BYTE GetMedianNum(BYTE *bArray, int iFilterLen);
	setGrayimage();
	BYTE p;
	BYTE neibor[9];
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			if ((j != 0) && (j != nWidth) && (i != 0) && (i != nHeight))
			{
				neibor[0] = m_pPixel[(i - 1)*nWidth + (j - 1)];
				neibor[1] = m_pPixel[(i - 1)*nWidth + j];
				neibor[2] = m_pPixel[(i - 1)*nWidth + (j + 1)];
				neibor[3] = m_pPixel[i*nWidth + (j - 1)];
				neibor[4] = m_pPixel[i*nWidth + j];
				neibor[5] = m_pPixel[i*nWidth + j + 1];
				neibor[6] = m_pPixel[(i + 1)*nWidth + (j - 1)];
				neibor[7] = m_pPixel[(i + 1)*nWidth + j];
				neibor[8] = m_pPixel[(i + 1)*nWidth + j + 1];
				p = GetMedianNum(neibor, 9);
			}
			else
			{
				p = grayImage[i*nWidth + j];
			}
			grayImage[i*nWidth + j] = p;
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

BYTE GetMedianNum(BYTE *bArray, int iFilterLen)
{
	int     i;
	int     j;
	BYTE bTemp;
	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen - 1; j++)
	{
		for (i = 0; i < iFilterLen - j - 1; i++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}
	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		bTemp = bArray[(iFilterLen + 1) / 2]; // 数组有奇数个元素，返回中间一个元素
	}
	else
	{ 
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;// 数组有偶数个元素，返回中间两个元素平均值
	}
	return bTemp;
}

void CyuhuachangView::OnFormerpic() //返回上一视图
{
	m_pPixel = former_pPixel;
	Invalidate(TRUE);
}

int Gaussian(float U,float D) 
{ //产生高斯样本，以U为均值，D为均方差
	double sum = 0;
	for (int i = 0; i < 12; i++)
	{
		sum += rand() / 32767.00;
		 //计算机中rand()函数为－32767～＋32767（2^15-1）
		 //故sum＋为0～1之间的均匀随机变量
	}
	return int(U + D*(sum - 6));  //产生均值为U，标准差为D的高斯分布的样本，并返回
}

void CyuhuachangView::OnAddnoiseguassian() //添加高斯噪声
{
	int mean = 0, var = 0;
	CDlgGuassiannoise gua;
	if (gua.DoModal() == IDOK)
	{
		mean = gua.mean_value;
		var = gua.variance;
	}
	srand((unsigned)time(NULL));  //种下随机种子
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			m_pPixel[i*nWidth + j]+=Gaussian(mean,var);
			if (m_pPixel[i*nWidth + j] > 255)
			{
				m_pPixel[i*nWidth + j]=255;
			}
			else if (m_pPixel[i*nWidth + j] < 0)
			{
				m_pPixel[i*nWidth + j] = 0;
			}
		}
	}
	Invalidate(TRUE);
}

void CyuhuachangView::OnAddnoisesaltpepper() //添加椒盐噪声
{
	int noise_num = 0,i=0,j=0;
	float temp;
	CDlgsaltpepper dlg;
	if (dlg.DoModal() == IDOK)
	{
		noise_num = nWidth*nHeight*(1-dlg.SNR);
	}
	srand((unsigned)time(NULL));  //种下随机种子
	for (int g = 0; g < noise_num; g++)
	{
		i = (rand() / 32767.00)*nWidth;
		j = (rand() / 32767.00)*nHeight;
		if (rand() / 32767.00 > 0.5 || rand() / 32767.00 < -0.5) 
		{
			m_pPixel[i*nWidth + j] = 0;
		}
		else {
			m_pPixel[i*nWidth + j] = 255;
		}
	}
	Invalidate(TRUE);
}

void CyuhuachangView::OnMosaic()
{
	setGrayimage();
	int i, j, ii, jj;
	BYTE temp;
	for (i = 0; i <nHeight; i++)
	{
		for (j = 0; j <nWidth; j++)
		{
			ii = int(i / 8) * 8;
			jj = int(j / 8) * 8;
			temp = m_pPixel[ii*nWidth + jj];
			grayImage[i*nWidth + j] = temp;
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnGraytocolormix()
{
	UpdateData();
	if (m_pPixel == NULL)
	{
		MessageBox(_T("没有源图像"));
		return;
	}
	//判断是否是真彩色图像
	bitCount = bmih.biBitCount;//获取位数
	if (bitCount == 24)
	{
		MessageBox(_T("灰度图无法处理！"));
		return;
	}
	//判断图像是否经过压缩
	if (bmih.biCompression != BI_RGB)
	{
		MessageBox(_T("不处理压缩过的图像！"));
		return;
	}
	//构造彩色图像文件头
	memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpFileHeader.bfOffBits = 54 + sizeof(RGBQUAD) * 256;
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmih.biSizeImage ;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfType = 0x4d42;

	bmpInfoHeader.biBitCount = 24;//变成彩色图像是24位
	bmpInfoHeader.biSize = 40;//信息头固定大小为40
	bmpInfoHeader.biHeight = bmih.biHeight;
	bmpInfoHeader.biWidth = bmih.biWidth;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bmih.biSizeImage;
	bmpInfoHeader.biXPelsPerMeter = bmih.biXPelsPerMeter;
	bmpInfoHeader.biYPelsPerMeter = bmih.biYPelsPerMeter;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 256;

	//RGBQUAD rgbquad[256];
	//满足长度和宽度都是4的整数倍
	nWidth = (bmpInfoHeader.biWidth / 4) * 4;
	if (nWidth < bmpInfoHeader.biWidth)
	{
		nWidth = nWidth + 4;
	}
	nHeight = (bmpInfoHeader.biHeight / 4) * 4;
	if (nHeight < bmpInfoHeader.biHeight)
	{
		nHeight = nHeight + 4;
	}
	int red, green, blue;
	BYTE gray;
	grayImage = new BYTE[3*nWidth*nHeight];
	for (int i = 0; i<nHeight; i++)
	{
		for (int j = 0; j<nWidth; j++)
		{
			gray = m_pPixel[(i*nWidth + j)]; 
			if (gray<128)
			{
				red = 0;
			}
			else if (gray<192)
			{
				red = 255 / 64 * (gray - 128);
			}
			else
			{
				red = 255;
			}  
			if (gray<64)
			{
				green = 255 / 64 * gray;
			}
			else if (gray<192)
			{
				green = 255;
			}
			else
			{
				green = -255 / 63 * (gray - 192) + 255;
			} 
			if (gray<64)
			{
				blue = 255;
			}
			else if (gray<128)
			{
				blue = -255 / 63 * (gray - 192) + 255;
			}
			else
			{
				blue = 0;
			}
			grayImage[3 * (i*nWidth + j)] = red;
			grayImage[3 * (i*nWidth + j) + 1] = green;
			grayImage[3 * (i*nWidth + j) + 2] = blue;
		}
	}
	CFileDialog dlg(false, _T("*.bmp"), NULL, OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT, _T("BMP files(*bmp)|*.bmp||"));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	s_path = dlg.GetPathName();
	CFile dib(s_path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	dib.Write(&bmpFileHeader, sizeof(BITMAPFILEHEADER));
	dib.Write(&bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	//dib.Write((void*)&rgbquad, sizeof(RGBQUAD) * 256);//写入调色板
	dib.Write(grayImage, 3*nHeight*nWidth);
	dib.Close();
	delete[]grayImage;
}

void CyuhuachangView::OnCharaterseg()
{
	CDlgCharaterSeg dlg;
	dlg.DoModal();
}

void CyuhuachangView::OnSharpeninglaplace() //八领域模板
{
	setGrayimage();
	int i, j;
	BYTE temp;
	for (i = 1; i <nHeight-1; i++)
	{
		for (j = 1; j <nWidth-1; j++)
		{
			temp = (BYTE)(8*m_pPixel[nWidth*i + j]-1 * m_pPixel[nWidth*(i-1) + j] - 1 * m_pPixel[nWidth*(i+1) + j] 
				- 1 * m_pPixel[nWidth*(i-1) + j-1] - 1 * m_pPixel[nWidth*i + j-1] - 1 * m_pPixel[nWidth*(i + 1) + j-1]
				- 1 * m_pPixel[nWidth*(i - 1) + j+1] - 1 * m_pPixel[nWidth*i + j+1]- 1*m_pPixel[nWidth*(i+1) + j + 1]);
			/*temp = (BYTE)(5 * m_pPixel[nWidth*i + j] - 1 * m_pPixel[nWidth*(i - 1) + j] - 1 * m_pPixel[nWidth*(i + 1) + j]
				 - 1 * m_pPixel[nWidth*i + j - 1]
				 - 1 * m_pPixel[nWidth*i + j + 1] );*/
			grayImage[nWidth*i + j] = m_pPixel[nWidth*i + j]+abs(temp);
			//grayImage[nWidth*i + j] = temp;
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnSharpeninggrad()//梯度锐化，阈值选择为20
{
	setGrayimage();
	int i, j;
	int t = 20;
	BYTE temp;
	for (i = 1; i <nHeight - 1; i++)
	{
		for (j = 1; j <nWidth - 1; j++)
		{
			//根据双向一次微分公式计算当前像素的灰度值
			temp = (BYTE)sqrt((float)((m_pPixel[nWidth*i + j]
				- m_pPixel[nWidth*i + (j - 1)])*(m_pPixel[nWidth*i + j]
					- m_pPixel[nWidth*i + (j - 1)])
				+ (m_pPixel[nWidth*i + j] - m_pPixel[nWidth*(i - 1) + j])*(m_pPixel[nWidth*i + j]
					- m_pPixel[nWidth*(i - 1) + j])));
			if (temp >= t)
			{
				if ((temp + 100)>255)
					grayImage[nWidth*i + j] = 255;
				else
					grayImage[nWidth*i + j] = temp + 100;
			}
			if (temp<t)
				grayImage[nWidth*i + j] = m_pPixel[nWidth*i + j];
		}
	}
	former_pPixel = m_pPixel;
	m_pPixel = grayImage;
	Invalidate(TRUE);
}

void CyuhuachangView::OnLinsencesegmentation()
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
	imshow("原图", imread(filename));
	Mat img = ImageLicence::licence_detect(myfilename,true); //检测车牌
	imshow("车牌照", img);
	waitKey(0);	
}

void CyuhuachangView::OnLisensestrseg()
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
	//imshow("原图", imread(filename));
	Mat img = ImageLicence::licence_detect(myfilename,false); //检测车牌
	//imshow("车牌照", img);
	Mat img1=ImageLicence::licence_seg(img, 100);
	imshow("车牌分割图", img1);
	waitKey(0);
}


void CyuhuachangView::On32838()
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
	//imshow("原图", imread(filename));
	Mat img = ImageLicence::licence_detect(myfilename, false); //检测车牌
															   //imshow("车牌照", img);
	Mat img1 = ImageLicence::licence_seg(img, 100);
	imshow("车牌分割图", img1);
	waitKey(0);
}
