#if !defined(AFX_MEIXIAOMING11VIEW_H__2FECFF76_43AF_46B4_A18B_8BA0E7133C37__INCLUDED_)
#define AFX_MEIXIAOMING11VIEW_H__2FECFF76_43AF_46B4_A18B_8BA0E7133C37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 
class CyuhuachangView : public CView
{
protected: 
	CyuhuachangView();
	DECLARE_DYNCREATE(CyuhuachangView)

// 特性
public:
	CyuhuachangDoc* GetDocument() const;
	BYTE* m_pPixel;
	BYTE* former_pPixel;
	BYTE* m_pBmInfo;
	BITMAPFILEHEADER* m_pBmfh;
	CString m_path;
	int bitCount;
	int nWidth, nHeight;
	long LineBytes;
	BITMAPINFOHEADER bmih;

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	BYTE* grayImage;
	CString s_path;

	int draw_x;
	int draw_y;

// 操作
public:
	void gray_linear_trans(double a, double b);
	void image_rotate(double ang);
	void Histogram_Statistic(int *gray, float *probability, int lWidth, int lHeight);
	void setGrayimage();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void set_drawpos(int x, int y);
	void save_threshold_figure(int threshold);
	
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CyuhuachangView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnViewIntensity();
	afx_msg void OnColor2gray();
	afx_msg void OnImagePanning();
	afx_msg void OnThreshold();
	afx_msg void OnReverse();
	afx_msg void OnPointTrans();
	afx_msg void OnPointthre();
	afx_msg void OnImageMirror();
	afx_msg void OnMutiply15();
	afx_msg void OnMutiply06();
	afx_msg void OnNonlinneartransDd255();
	afx_msg void OnEdgeSobel();
	afx_msg void OnEdgeLaplace();
	afx_msg void OnAveragefilter();
	afx_msg void OnEdgeRoberts();
	afx_msg void OnImageRotate();
	afx_msg void OnHistogramequalization();
	afx_msg void OnHistogrammatch();
	afx_msg void OnMediumfilter();
	afx_msg void OnFormerpic();
	afx_msg void OnAddnoiseguassian();
	afx_msg void OnAddnoisesaltpepper();
	afx_msg void OnMosaic();
	afx_msg void OnGraytocolormix();
	afx_msg void OnCharaterseg();
	afx_msg void OnSharpeninglaplace();
	afx_msg void OnSharpeninggrad();
	afx_msg void OnLinsenceseg();
	afx_msg void OnLinsencesegmentation();
	afx_msg void OnLisensestrseg();
	afx_msg void On32838();
};

#ifndef _DEBUG  // yuhuachangView.cpp 中的调试版本
inline CyuhuachangDoc* CyuhuachangView::GetDocument() const
   { return reinterpret_cast<CyuhuachangDoc*>(m_pDocument); }
#endif

#endif 

