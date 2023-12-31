﻿#include "stdafx.h"
#include "QtGrabWindow.h"
#include <QElapsedTimer>
#include <QTest>
#include <Winuser.h>
#include <commctrl.h>
#include <Winbase.h>

#include "public.hpp"
using namespace cv;
void RemoteHeaderItem(HWND hWnd);
void RemoteClick(HWND hWnd, POINT p,UINT Msg);

//Windows系统中使用DXGI截取桌面图像 此代码中DXGI获取图像来源于 https://www.cnblogs.com/TechNomad/p/17428347.html 
//Windows系统中使用GDI截取桌面图像
//https://maimai.cn/article/detail?fid=1745138640&efid=LSyZuQed3d1ClZ4gIMNPzQ

QtGrabWindow::QtGrabWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(QStringLiteral("Qt之grabWindow实现截图功能"));
	clickP = QPoint(0, 0);

	className = TEXT("UnityWndClass");//UnrealWindow UnityWndClass
	winName = TEXT("IdleSpiral");//剑灵 IdleSpiral
	hwnd = FindWindow(className, winName);
	::SetForegroundWindow(hwnd);
	m_pScreen = QApplication::primaryScreen();
	bx = 65536 / m_pScreen->size().width();
	by = 65536 / m_pScreen->size().height();
	startTimer(10);   //1秒25帧
	timer = new QTimer(this);
	bool isConnect = connect(timer, &QTimer::timeout, this, &QtGrabWindow::slotTimer);
	timer->start(1000);
	setMouseTracking(true);
	this->centralWidget()->setMouseTracking(true);
	//drawOnce();
	qDebug() << "test vs update";
	QTimer::singleShot(1000, this, &QtGrabWindow::getPosition);
}

QtGrabWindow::~QtGrabWindow()
{}
void QtGrabWindow::paintEvent(QPaintEvent * e)
{
#if 0
    //LPCWSTR className = TEXT("UnityWndClass");//UnrealWindow UnityWndClass
    //LPCWSTR winName = TEXT("IdleSpiral");//剑灵 IdleSpiral
	//HWND hwnd = FindWindow(className, winName);

#if GDI
	QPixmap pix = QPixmap::fromImage(myGrabWindow((WId)hwnd,false));
	if (!pix)
	{
		return;
	}
#else DXGI
	QPixmap pix = QPixmap::fromImage(GetDesktopFrame());
#endif //DXGI
	resize(pix.size());

	QImage img2 = pix.toImage();
	// QImage --->Mat的转换
	cv::Mat mat2 = cv::Mat(img2.height(), img2.width(), CV_8UC4, img2.bits(), img2.bytesPerLine());
	//cv::Mat src_mat = cv::imdecode(pic_arr, cv::IMREAD_UNCHANGED);
	cv::imshow("123", mat2);

	//绘制截屏
	QPainter p;
	p.begin(this);
	p.drawImage(QPoint(0, 0), pix.toImage());
	p.end();
#endif
}
//GDI和DXGI这两种截图方法，同时在Qt中实现了这几种截图方法，并进行了性能和效果上的比较

//GDI： 因为windows的应用程序是不能直接访问图形硬件（GPU）的，所以在windows中实现了一系列交互的接口（Graphics Device Interface），也就是我们所说的GDI。GDI 截图就是通过屏幕的DC（Device context，设备的描述表）获取到当前屏幕的位图数据。这种截图方法的好处是：不受windows版本限制，基本兼容各版本的系统；
QImage QtGrabWindow::myGrabWindow(WId winId, bool needMouse) 
{
	QSize windowSize;    
	int x = 0;        
	int y = 0;        
	HWND hwnd = reinterpret_cast<HWND>(winId);        
	if (!hwnd) { return QImage(); }    
	//以下部分在获取这个句柄应用的size        
	RECT r;        
	GetClientRect(hwnd, &r);        
	windowSize = QSize(r.right - r.left, r.bottom - r.top);        
	int width = windowSize.width();
	int height = windowSize.height();
	HDC display_dc = GetDC(nullptr);        
	HDC bitmap_dc = CreateCompatibleDC(display_dc);        
	HBITMAP bitmap = CreateCompatibleBitmap(display_dc, width, height);        
	HGDIOBJ null_bitmap = SelectObject(bitmap_dc, bitmap);        
	BOOL imgFlag = FALSE;
	HDC window_dc = GetDC(hwnd);  //获取句柄DC    
	//imgFlag = BitBlt(bitmap_dc, 0, 0, width, height, window_dc, x, y, SRCCOPY | CAPTUREBLT); //使用Bitblt方法    上面是用Bitblt方法实现的，有个StretchBlt方法，和Bitblt类似，只是增加了生成图片的缩放。
	imgFlag = printWindow()(hwnd, bitmap_dc, PW_CLIENTONLY | PW_RENDERFULLCONTENT);


	ReleaseDC(hwnd, window_dc);        
	if (needMouse) {  //需要截取鼠标的话单独处理                
		CURSORINFO curinfo;                
		curinfo.cbSize = sizeof(curinfo);                
		GetCursorInfo(&curinfo);                
		POINT screenPt = curinfo.ptScreenPos;                
		ScreenToClient(hwnd, &screenPt);                
		DrawIcon(bitmap_dc, screenPt.x, screenPt.y, curinfo.hCursor);       
	}        
	SelectObject(bitmap_dc, null_bitmap);        
	DeleteDC(bitmap_dc);        
	QImage image;        
	if (imgFlag) {                
		image = QImage::fromHBITMAP(bitmap);
	}        
	DeleteObject(bitmap);        
	ReleaseDC(nullptr, display_dc);        
	return image;
}
//因为PrintWindow不是所有电脑都支持的，所以这里使用了Qt的QLibrary动态加载，来测试是否支持这个函数：只需要将之前使用bitblt的地方换成printWindow，就可以了：
PtrPrintWindow QtGrabWindow::printWindow()
{ 
	static bool hasTestPrintWindowFunction = false;      
	static PtrPrintWindow printWindowFunnction = nullptr;      
	if (hasTestPrintWindowFunction) 
	{ 
		return printWindowFunnction; 
	}       
	hasTestPrintWindowFunction = true;      
	printWindowFunnction = reinterpret_cast<PtrPrintWindow>(QLibrary::resolve("user32.dll", "PrintWindow"));       
	return printWindowFunnction; 
}
//没有勾选使用printWindow的是使用的Bitblt方法，可以看到使用Bitblt的截图方法ppt只有个外围的框架，没截取到里面内容分，右下角完全黑的是我edge浏览器放的一个当前时间的网页。而使用print后，两个均能截出来。
//printwindow其实是给对方发了一个WN_PAINTER消息，对方响应了这个消息就能截出来，频率高了的话可能出现闪屏现象。


//DXGI：
//DXGI其实是Microsoft DirectX Graphics Infrastructure的缩写，是微软提供的一种可以在win8及以上系统使用的图形设备接口。它负责枚举图形适配器、枚举显示模式、选择缓冲区格式、在进程之间共享资源以及将呈现的帧传给窗口或监视器以供显示。其直接和硬件设备进行交互，具有很高的效率和性能。

//DXGI最大的有点就是效率高，CPU占用非常低。
//它的缺点是：
//1、有版本要求，win8以上才支持。
//2、因为采集需要获取设备的adapter，所以无法采集桌面窗口。
//这个要注意：初始化的线程一定要在一个没有处理过UI的线程，否则会初始化失败
#ifdef DXGI_TEST
bool QtGrabWindow::Init() {
	int adaptIndex = 0, outputIndex = 0;        
	//QList<ScreenOutput> list;        
	//bool flag = getScreens(list); //获取屏幕列表        
	//if (!flag || list.size() == 0) 
	//{                
	//	return false;        
	//}        
	//adaptIndex = list.at(0).adaptorIndex;        
	//outputIndex = list.at(0).outputIndex;        
	HRESULT hr = S_OK;        
	if (m_bInit) 
	{        
		return FALSE;        
	}        
	// Driver types supported        
	D3D_DRIVER_TYPE DriverTypes[] =       
	{                
		D3D_DRIVER_TYPE_HARDWARE,                
		D3D_DRIVER_TYPE_WARP,                
		D3D_DRIVER_TYPE_REFERENCE,        
	};        
	UINT NumDriverTypes = ARRAYSIZE(DriverTypes);        
	// Feature levels supported        
	D3D_FEATURE_LEVEL FeatureLevels[] =        
	{                
		D3D_FEATURE_LEVEL_11_0,                
		D3D_FEATURE_LEVEL_10_1,                
		D3D_FEATURE_LEVEL_10_0,                
		D3D_FEATURE_LEVEL_9_1        
	};        
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);        
	D3D_FEATURE_LEVEL FeatureLevel;        // 创建 D3D device 

	for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; ++DriverTypeIndex) {        
		hr = D3D11CreateDevice(NULL, DriverTypes[DriverTypeIndex], NULL, 0, FeatureLevels, NumFeatureLevels, D3D11_SDK_VERSION, &m_hDevice, &FeatureLevel, &m_hContext);        
		if (SUCCEEDED(hr)) 
		{            
			break;                
		}        
	}        
	if (FAILED(hr)) 
	{        
		return FALSE;        
	}            
	// 获取 DXGI device        
	IDXGIDevice *hDxgiDevice = NULL;        
	hr = m_hDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&hDxgiDevice));        
	if (FAILED(hr))
	{                
		return FALSE;        
	}        
	// 获取 DXGI adapter        
	IDXGIFactory* pFactory;        
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));        
	IDXGIAdapter* hDxgiAdapter = nullptr;        
	hr = pFactory->EnumAdapters(0, &hDxgiAdapter);      
	hDxgiDevice->Release();
	if (FAILED(hr)) 
	{                 
		return FALSE;         
	}        
	int nOutput = outputIndex;         
	IDXGIOutput *hDxgiOutput = NULL;         
	hr = hDxgiAdapter->EnumOutputs(nOutput, &hDxgiOutput);         
	hDxgiAdapter->Release();
	if (FAILED(hr)) 
	{                 
		return FALSE;         
	}
	hDxgiOutput->GetDesc(&m_dxgiOutDesc);         
	IDXGIOutput1 *hDxgiOutput1 = NULL;         
	hr = hDxgiOutput->QueryInterface(__uuidof(hDxgiOutput1), reinterpret_cast<void**>(&hDxgiOutput1));         
	hDxgiOutput->Release();
	if (FAILED(hr)) 
	{                 
		return FALSE;        
	}        
	hr = hDxgiOutput1->DuplicateOutput(m_hDevice, &m_hDeskDupl);         
	hDxgiOutput1->Release();
	if (FAILED(hr)) 
	{                 
		return FALSE;         
	}         
	// 初始化成功         
	m_bInit = TRUE;         
	return TRUE; 
}
bool QtGrabWindow::QueryFrame(QRect& rect, void* pImgData, INT& nImgSize) {
	if (!m_bInit /*|| !AttatchToThread()*/) 
	{ 
		return FALSE; 
	}       
	DXGI_OUTDUPL_FRAME_INFO FrameInfo;        
	IDXGIResource* hDesktopResource = NULL;   
	HRESULT hr = m_hDeskDupl->AcquireNextFrame(20, &FrameInfo, &hDesktopResource);        
	if (FAILED(hr)) 
	{ hDesktopResource = nullptr;                return TRUE; }        // query next frame staging buffer         
	ID3D11Texture2D *hAcquiredDesktopImage = NULL;         
	hr = hDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&hAcquiredDesktopImage));         
	hDesktopResource->Release();
	if (FAILED(hr)) 
	{                 return FALSE;       }         
	// copy old description         
	D3D11_TEXTURE2D_DESC frameDescriptor;        
	hAcquiredDesktopImage->GetDesc(&frameDescriptor);         // create a new staging buffer for fill frame image        
	ID3D11Texture2D *hNewDesktopImage = NULL;         
	frameDescriptor.Usage = D3D11_USAGE_STAGING;         
	frameDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;         
	frameDescriptor.BindFlags = 0;         
	frameDescriptor.MiscFlags = 0;         
	frameDescriptor.MipLevels = 1;         
	frameDescriptor.ArraySize = 1;         
	frameDescriptor.SampleDesc.Count = 1;     
	hr = m_hDevice->CreateTexture2D(&frameDescriptor, NULL, &hNewDesktopImage);          
	if (FAILED(hr)) 
	{                  
		hAcquiredDesktopImage->Release();
		m_hDeskDupl->ReleaseFrame();                  
		return FALSE;         
	}          
	// copy next staging buffer to new staging buffer     
	m_hContext->CopyResource(hNewDesktopImage, hAcquiredDesktopImage);          
	hAcquiredDesktopImage->Release();
	m_hDeskDupl->ReleaseFrame();          // create staging buffer for map bits          
	static IDXGISurface *hStagingSurf = NULL;          
	if (hStagingSurf == NULL) 
	{                  
		hr = hNewDesktopImage->QueryInterface(__uuidof(IDXGISurface), (void **)(&hStagingSurf));                  
		hNewDesktopImage->Release();
		if (FAILED(hr)) 
		{                          
			return FALSE;                 
		}         
	}          
	// copy bits to user space          
	DXGI_MAPPED_RECT mappedRect;          
	hr = hStagingSurf->Map(&mappedRect, DXGI_MAP_READ);          
	if (SUCCEEDED(hr)) 
	{          
		QRect desttopRect(m_dxgiOutDesc.DesktopCoordinates.left, m_dxgiOutDesc.DesktopCoordinates.top, m_dxgiOutDesc.DesktopCoordinates.right - m_dxgiOutDesc.DesktopCoordinates.left, m_dxgiOutDesc.DesktopCoordinates.bottom - m_dxgiOutDesc.DesktopCoordinates.top);                  
		copyImageByRect((char*)mappedRect.pBits, desttopRect.size(), (char*)pImgData, nImgSize, rect);                  
		hStagingSurf->Unmap();         
	}         
	hStagingSurf->Release();
	return SUCCEEDED(hr);  
}
bool QtGrabWindow::copyImageByRect(char* src, QSize size, char* dst,int imgSize, QRect rect)
{
	rect.setSize(size);
	dst = src;
	imgSize = size.width() * size.height();
	return true;
}
#endif // 0

#ifdef DXGI
bool QtGrabWindow::InitD3D11Device()
{
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = std::extent<decltype(driverTypes)>::value;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT numFeatureLevels = std::extent<decltype(featureLevels)>::value;
	D3D_FEATURE_LEVEL featureLevel;

	for (UINT i = 0; i < numDriverTypes; ++i)
	{
		HRESULT hr = D3D11CreateDevice(nullptr, driverTypes[i], nullptr, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &m_pDevice, &featureLevel, &m_pDeviceContext);
		if (SUCCEEDED(hr)) {
			break;
		}
	}

	if (m_pDevice == nullptr || m_pDeviceContext == nullptr) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "InitD3D11Device Failed";
		return false;
	}

	return true;

}
bool QtGrabWindow::InitDuplication()
{
	HRESULT hr = S_OK;

	/* IDXGIDevice 接口是 DXGI中的一个接口，用于表示一个设备对象，该设备对象是与 Direct3D 设备相关联的。 */
	IDXGIDevice* dxgiDevice = nullptr;
	hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	if (FAILED(hr)) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "QueryInterface Failed:" << hr;
		return false;
	}

	if (dxgiDevice == nullptr) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "dxgiDevice is  nullptr";
		return false;
	}

	IDXGIAdapter* dxgiAdapter = nullptr;
	/* 获取与设备相关联的适配器对象 */
	hr = dxgiDevice->GetAdapter(&dxgiAdapter);
	dxgiDevice->Release();
	if (FAILED(hr)) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "GetAdapter Failed:" << hr;
		return false;
	}

	if (dxgiAdapter == nullptr) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "dxgiAdapter is  nullptr";
		return false;
	}

	UINT output = 0;
	IDXGIOutput* dxgiOutput = nullptr;
	while (true)
	{
		hr = dxgiAdapter->EnumOutputs(output++, &dxgiOutput);
		if (hr == DXGI_ERROR_NOT_FOUND) {
			return false;
		}
		else {
			if (dxgiOutput == nullptr) {
				qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "dxgiOutput is  nullptr";
				break;
			}

			DXGI_OUTPUT_DESC desc;
			dxgiOutput->GetDesc(&desc);
			m_screenWidth = desc.DesktopCoordinates.right - desc.DesktopCoordinates.left;
			m_screenHeight = desc.DesktopCoordinates.bottom - desc.DesktopCoordinates.top;
			break;
		}
	}
	dxgiAdapter->Release();

	/* IDXGIOutput1 接口是 DXGI中的一个接口，它是 IDXGIOutput 接口的扩展版本。IDXGIOutput1 提供了一些额外的方法和功能，
	 * 用于与显示输出（display output）进行交互和控制。 */
	IDXGIOutput1* dxgiOutput1 = nullptr;
	hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&dxgiOutput1));
	dxgiOutput->Release();
	if (FAILED(hr)) {
		return false;
	}

	if (dxgiOutput1 == nullptr) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "dxgiOutput1 is  nullptr";
		return false;
	}

	hr = dxgiOutput1->DuplicateOutput(m_pDevice, &m_pDuplication);
	dxgiOutput1->Release();
	if (FAILED(hr)) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "DuplicateOutput Failed";
		return false;
	}

	return true;
}
QImage QtGrabWindow::GetDesktopFrame(QString fileName)
{
	QImage image;
	HRESULT hr = S_OK;
	DXGI_OUTDUPL_FRAME_INFO frameInfo;
	IDXGIResource* resource = nullptr;
	ID3D11Texture2D* acquireFrame = nullptr;
	ID3D11Texture2D* texture = nullptr;

	if (m_pDuplication == nullptr) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "m_pDuplication is  nullptr";
		return QImage();
	}

	hr = m_pDuplication->AcquireNextFrame(0, &frameInfo, &resource);
	if (FAILED(hr)) {
		return QImage();
	}

	if (resource == nullptr) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "resource is  nullptr";
		return QImage();
	}

	hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&acquireFrame));
	resource->Release();
	if (FAILED(hr)) {
		return QImage();
	}

	if (acquireFrame == nullptr) {
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "acquireFrame is  nullptr";
		return QImage();
	}

	D3D11_TEXTURE2D_DESC desc;
	acquireFrame->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	m_pDevice->CreateTexture2D(&desc, NULL, &texture);
	if (texture) {
		/* 将图像数据从显存中拷贝到内存中 */
		m_pDeviceContext->CopyResource(texture, acquireFrame);
		image = this->CopyDesktopImage(texture);
		//image.save(fileName, "PNG");
	}
	acquireFrame->Release();

	hr = m_pDuplication->ReleaseFrame();
	if (FAILED(hr)) {
		return QImage();
	}

	return image;
}
QImage QtGrabWindow::CopyDesktopImage(ID3D11Texture2D* texture)
{
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	m_pDeviceContext->Map(texture, 0, D3D11_MAP_READ, 0, &mapped_resource);

	size_t imageSize = desc.Width * desc.Height * 4;
	uint8_t* rgba = (uint8_t*)malloc(imageSize);
	if (rgba == nullptr)
	{
		return QImage();
	}
	memset(rgba, 0, imageSize);
	uint8_t* pData = (uint8_t*)mapped_resource.pData;
	for (size_t i = 0; i < desc.Height; i++)
	{
		memcpy(rgba + i * desc.Width * 4, pData + i * mapped_resource.RowPitch, desc.Width * 4);
	}
	QImage image(static_cast<uchar*>(rgba), m_screenWidth, m_screenHeight, QImage::Format_ARGB32);
	//QImage image(static_cast<uchar*>(mapped_resource.pData), m_screenWidth, m_screenHeight, QImage::Format_ARGB32);

	texture->Release();
	texture = nullptr;

	return image;
}
#endif 

void QtGrabWindow::timerEvent(QTimerEvent* e)
{
    update();  //更新窗口
}
QImage QtGrabWindow::findPicture(QImage src, QImage child)
{

	QImage cChi = child.convertToFormat(QImage::Format_RGB32);
	//cSrc.setAlphaChannel(child.convertToFormat(QImage::Format_Alpha8));
	Mat srcImg = QImage2cvMat(src);
	Mat templateImg = QImage2cvMat(cChi);

	//图像匹配 --> 大图中找小图
#if 1
	//Mat image1 = imread("/test1.png", IMREAD_GRAYSCALE);
	//Mat image2 = imread("/test2.png", IMREAD_GRAYSCALE);
	//qDebug() << QDir::currentPath();
	//Mat templateImg = imread("./test2.png");
	//Mat srcImg = imread("./test1.png");
	//Mat dst = srcImg.clone();
	//imshow("src", srcImg);

	//int width = srcImg.cols - templateImg.cols + 1;//result宽度
	//int height = srcImg.rows - templateImg.rows + 1;//result高度

	//Mat resultImg(height, width, CV_32FC1);//创建结果映射图像
	Mat resultImg(srcImg.rows - templateImg.rows + 1, srcImg.cols - templateImg.cols + 1, CV_32FC1); //构建结果矩阵

	//matchTemplate(srcImg, templateImg, resultImg, TM_SQDIFF); //平方差匹配法(最好匹配0)
	//matchTemplate(srcImg, templateImg, resultImg, TM_SQDIFF_NORMED); //归一化平方差匹配法(最好匹配0)
	//matchTemplate(srcImg, templateImg, resultImg, TM_CCORR); //相关匹配法(最坏匹配0)
	//matchTemplate(srcImg, templateImg, resultImg, TM_CCORR_NORMED); //归一化相关匹配法(最坏匹配0)
	//matchTemplate(srcImg, templateImg, resultImg, TM_CCOEFF); //系数匹配法(最好匹配1)
	matchTemplate(srcImg, templateImg, resultImg, TM_CCOEFF_NORMED);//化相关系数匹配,最佳值1
	//imshow("result", resultImg);
	//normalize(resultImg, resultImg, 0, 1, NORM_MINMAX, -1);//归一化到0-1范围

	double minValue, maxValue;
	Point minLoc, maxLoc;
	minMaxLoc(resultImg, &minValue, &maxValue, &minLoc, &maxLoc);
	//qDebug() << "minValue=" << minValue ;
	//qDebug() << "maxValue=" << maxValue ;
	if (maxValue < 0.95)//置信度
	{
		clickP = QPoint(0, 0);
		return cvMat2QImage(srcImg);
	}
	//匹配结果的四个顶点
	Point pt1(maxLoc.x, maxLoc.y);
	Point pt2(maxLoc.x + templateImg.cols, maxLoc.y);
	Point pt3(maxLoc.x, maxLoc.y + templateImg.rows);
	Point pt4(maxLoc.x + templateImg.cols, maxLoc.y + templateImg.rows);

	clickP = QPoint(pt1.x + (pt2.x - pt1.x)/2, pt1.y + (pt4.y - pt2.y)/2);

	//画线
	line(srcImg, pt1, pt2, cv::Scalar(0, 255, 0), 11, 1);
	line(srcImg, pt2, pt4, cv::Scalar(0, 255, 0), 11, 1);
	line(srcImg, pt4, pt3, cv::Scalar(0, 255, 0), 11, 1);
	line(srcImg, pt3, pt1, cv::Scalar(0, 255, 0), 11, 1);
	//rectangle(srcImg, maxLoc, Point(maxLoc.x + templateImg.cols, maxLoc.y + templateImg.rows), Scalar(0, 255, 0), 2, 8);
	//imshow("dst", dst);
	//imshow("result", srcImg);
	//waitKey();
#endif
#if 0
	//Mat image = imread("./test1.png");  //读取原图
	//Mat templ = imread("./test2.png", 0);  //读取模板图的灰度图像

	Mat src1;
	cvtColor(image, src1, COLOR_RGB2GRAY); //将原图转换为灰度图像
	imshow("src", image);
	imshow("templ", templ);
	imshow("src1", src1);
#endif // 0
#if 0



	Mat result(src1.rows - templ.rows + 1, src1.cols - templ.cols + 1, CV_32FC1); //构建结果矩阵
	matchTemplate(src1, templ, result, TM_CCOEFF_NORMED); //模板匹配

	double dMaxVal; //分数最大值	
	Point ptMaxLoc; //最大值坐标
	minMaxLoc(result, 0, &dMaxVal, 0, &ptMaxLoc); //寻找结果矩阵中的最大值

	//匹配结果的四个顶点
	Point pt1(ptMaxLoc.x, ptMaxLoc.y);
	Point pt2(ptMaxLoc.x + templ.cols, ptMaxLoc.y);
	Point pt3(ptMaxLoc.x, ptMaxLoc.y + templ.rows);
	Point pt4(ptMaxLoc.x + templ.cols, ptMaxLoc.y + templ.rows);

	//画线
	line(image, pt1, pt2, cv::Scalar(0, 255, 0), 11, 1);
	line(image, pt2, pt4, cv::Scalar(0, 255, 0), 11, 1);
	line(image, pt4, pt3, cv::Scalar(0, 255, 0), 11, 1);
	line(image, pt3, pt1, cv::Scalar(0, 255, 0), 11, 1);


	imshow("image", image);
	//imwrite("img.jpg", image);
	waitKey();

#endif // 0
	return cvMat2QImage(srcImg);
}

void QtGrabWindow::getPosition()
{
	QPixmap pix = QPixmap::fromImage(myGrabWindow((WId)hwnd, false));
	if (!pix)
	{
		return;
	}
	//::SetForegroundWindow(hwnd);
	QImage showImg = findPicture(pix.toImage(), QImage(":/QtComposeResultCalculator/Resource/test2.png"));
}
void QtGrabWindow::slotTimer()
{
	QElapsedTimer e;
	e.start();

	QPixmap pix = QPixmap::fromImage(myGrabWindow((WId)hwnd, false));
	if (!pix)
	{
		return;
	}
	if (clickP.x() < 0 || clickP.x() > m_pScreen->size().width() || clickP.y() < 0 || clickP.y() > m_pScreen->size().height())
	{
		return;
	}
	
	::SetForegroundWindow(hwnd);
	qDebug() << "GrabWindow to image timeElapsed: " << e.elapsed();
	//e.restart();
	//QImage showImg = findPicture(pix.toImage(), QImage(":/QtComposeResultCalculator/Resource/test2.png"));
	//qDebug() << "findPicture timeElapsed: " << e.elapsed();
	e.restart();

	QWindow* windowLogin = QWindow::fromWinId((WId)hwnd);
	windowLogin->setFramePosition(QPoint(0,31));
	//QSize size = windowLogin->size();
	POINT pos = { clickP.x(),clickP.y() };
	//GetCursorPos(&pos);
	//qDebug() << "x" << pos.x << "y" << pos.y;



	//qDebug() << mapToGlobal(clickP);
	//QTest::mouseMove(windowLogin, mapToGlobal(clickP), 100);
	//QTest::mouseClick(windowLogin, Qt::LeftButton, Qt::NoModifier, clickP, 100);
	//QTest::keyClick(windowLogin, Qt::Key_W, Qt::NoModifier,100);


	// 按下'w'键
	//keybd_event(119, 0, 0, 0);
	//keybd_event(119, 0, KEYEVENTF_KEYUP, 0);
	// 
	//showImg.setPixelColor(1207, 989-31,QColor(0,0,255));
	QColor pixcolor = pix.toImage().pixelColor(QPoint(1207, 889 - 31));
	if (pixcolor.red() != 255)
	{
		clickP.setY(clickP.y() + 31);
		qDebug() << clickP;
		Sleep(1500);
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MOVE, clickP.x() * bx, clickP.y() * by, 0, 0);
		//::ScreenToClient(hwnd, &pos); // 把pos转成相对于窗口客户区的坐标
		//qDebug() << "x1" << pos.x << "y1" << pos.y;
		::SetFocus(hwnd);
		//RemoteHeaderItem(hwnd);
		//传递过去的消息应该是不正确的
		//RemoteClick(hwnd, pos, WM_ACTIVATE);
		//RemoteClick(hwnd, pos, WM_SETFOCUS);
		//RemoteClick(hwnd, pos, WM_MOUSEACTIVATE);
		//RemoteClick(hwnd, pos, WM_MOUSEMOVE);
		//RemoteClick(hwnd, pos, WM_INPUT);
		//RemoteClick(hwnd, pos, WM_LBUTTONDOWN);
		//RemoteClick(hwnd, pos, WM_INPUT);
		//RemoteClick(hwnd, pos, WM_LBUTTONUP);
		//RemoteClick(hwnd, pos, WM_MOUSEMOVE);

		//bool isS0 = ::SendMessage(hwnd, WM_INPUT, 0, MAKELPARAM((long)clickP.x(), (long)clickP.y()));
		//bool isS1 = ::SendMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM((long)clickP.x(), (long)clickP.y()));
		//bool isS2 = ::SendMessage(hwnd, WM_INPUT, 0, MAKELPARAM((long)clickP.x(), (long)clickP.y()));
		//bool isS3 = ::SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM((long)clickP.x(), (long)clickP.y()));
		//bool isS = ::SendMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM(pos.x, pos.y));
		//bool isS1 = ::SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(pos.x, pos.y));
		//qDebug() << "isS" << isS;
		//qDebug() << "isS1" << isS1;
		//qDebug() << "error:" << GetLastError();
		QTimer::singleShot(1000, this, &QtGrabWindow::clickOther);
	}
	qDebug() << "R" << QString::number(pixcolor.red());//255 127
	//qDebug() << "G" << QString::number(pixcolor.green());//0 15
	//qDebug() << "B" << QString::number(pixcolor.blue());//0 15

#if 0
	//QImage img2 = pix.toImage();
	int mX = 1550;
	int mY = 460;
	int mW = 100;
	int mH = 100;
	mX = std::min(mX, pix.rect().bottomRight().x());
	mY = std::min(mY, pix.rect().bottomRight().y());
	mW = std::min(mW, pix.width());
	mH = std::min(mH, pix.height());
	QImage img2 = pix.copy(mX, mY, mW, mH).toImage();
	resize(img2.size());
#endif
#ifdef ISPAINT
	//绘制截屏
	QPalette palette = this->palette();
	QBrush brush = palette.brush(QPalette::Window);
	brush.setTextureImage(showImg);
	palette.setBrush(QPalette::Window, brush);
	this->setPalette(palette);
	resize(pix.size());
#endif // DEBUG
	qDebug() << "ohter timeElapsed: " << e.elapsed();
}
void RemoteClick(HWND hWnd, POINT pP, UINT Msg)
{
	// Get hProcess
	DWORD pid;
	GetWindowThreadProcessId(hWnd, &pid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	//PrefetchVirtualMemory
	POINT* remote_ptr = (POINT*)VirtualAllocEx(hProcess, 0, sizeof(POINT), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!remote_ptr)
	{
		//::MessageBox(NULL, "无法分配内存!", "错误!", NULL);
		qDebug() << "无法分配内存!";
		QMessageBox::warning(nullptr,"错误!","无法分配内存!");
		CloseHandle(hProcess);
		return;
	}
	//
	MSG msg;
	msg.hwnd = hWnd;
	msg.message = Msg;
	msg.wParam = 0; 
	msg.lParam = MAKELPARAM(pP.x, pP.y);
	msg.time = 50; 
	msg.pt = pP;

	//
	{
		WriteProcessMemory(hProcess, remote_ptr, &msg, sizeof(MSG), 0);
		::SendMessage(hWnd, Msg, 0, (LPARAM)remote_ptr);
		//		afxDump << ItemBuf << "\n";
	}
	//释放内存
	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, remote_ptr, 0, MEM_RELEASE);
}

void RemoteHeaderItem(HWND hWnd)
{
	// GetItemCount
	int nHeaderCount = ::SendMessage(hWnd, HDM_GETITEMCOUNT, 0, 0);
	// Get hProcess
	DWORD pid;
	GetWindowThreadProcessId(hWnd, &pid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	//PrefetchVirtualMemory
	HDITEM* pHeaderItem = (HDITEM*)VirtualAllocEx(hProcess, NULL, sizeof(HDITEM), MEM_COMMIT, PAGE_READWRITE);
	char* pItem = (char*)VirtualAllocEx(hProcess, NULL, 80, MEM_COMMIT, PAGE_READWRITE);
	if ((!pHeaderItem) || (!pItem))
	{
		//::MessageBox(NULL, "无法分配内存!", "错误!", NULL);
		CloseHandle(hProcess);
		return;
	}
	//
	HDITEM HeaderItem;
	HeaderItem.cchTextMax = 80;
	HeaderItem.mask = HDI_TEXT;
	HeaderItem.pszText = reinterpret_cast<LPWSTR>(pItem);
	//
	char ItemBuf[80];
	for (int i = 0; i < nHeaderCount; i++)
	{
		HeaderItem.iOrder = i;
		WriteProcessMemory(hProcess, pHeaderItem, &HeaderItem, sizeof(HDITEM), NULL);
		::SendMessage(hWnd, HDM_GETITEM, i, (LPARAM)pHeaderItem);
		ReadProcessMemory(hProcess, pItem, ItemBuf, 80, NULL);
		//		afxDump << ItemBuf << "\n";
	}
	//释放内存
	CloseHandle(hProcess);
	VirtualFreeEx(hProcess, pHeaderItem, 0, MEM_RELEASE);
	VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
}
BOOL RegisitKeyBord(HWND hwnd)
{
	if (NULL == hwnd)
		return false;

	RAWINPUTDEVICE rid;  //设备信息
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02; //键盘   rid.usUsagePage = 0x01; rid.usUsage = 0x02; 为鼠标
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = hwnd;

	return RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));

#ifdef _THIS_OTHER_CODE
	HGLOBAL hRaw = ::GlobalAlloc(GHND, sizeof(RAWINPUT));
	RAWINPUT* pRaw = reinterpret_cast<RAWINPUT*>(::GlobalLock(hRaw));
	// initialize the structure using pRaw
	::GlobalUnlock(hRaw);
	// use hRaw as the LPARAM
#endif
}
//将下面的代码设置到主函数,调用函数进行bitmap图片的保存
#ifdef DXGI
int main(int argc, char* argv[])
{
	std::shared_ptr<QtGrabWindow> screen_capture = std::make_shared<QtGrabWindow>();
	if (!screen_capture.get()->InitD3D11Device()) {
		return  -1;
	}

	if (!screen_capture.get()->InitDuplication()) {
		return -1;
	}

	int counts = 0;
	/* 每隔1秒获取一次图像 */
	while (counts < 5) {
		QString fileName = QString("%1.png").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
		screen_capture.get()->GetDesktopFrame(fileName);

		counts++;
		Sleep(1000);
	}
}
#endif
void QtGrabWindow::clickOther()
{
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MOVE, clickP.x()* bx, (clickP.y() - 70)* by, 0, 0);
	//::SendMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM(clickP.x(), clickP.y()-70));
	//::SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(clickP.x(), clickP.y()-70));
	Sleep(200);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MOVE, clickP.x() * bx, (clickP.y() + 70) * by, 0, 0);
	//::SendMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM(clickP.x(), clickP.y()+70));
	//::SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(clickP.x(), clickP.y()+70));
	clickP.setY(clickP.y() - 31);
}
void QtGrabWindow::mouseMoveEvent(QMouseEvent* event)
{
#if 0
	qDebug() << "X" << QString::number(event->x());
	qDebug() << "Y" << QString::number(event->y());

	//    ui->showX->setText(QString::number(QCursor().pos().x()));//以电脑屏幕左上角为0点
	//    ui->showY->setText(QString::number(QCursor().pos().y()));

	//  ui->showdata->setText(tr("(%1,%2)").arg(event->x()).arg(event->y())); //哦。。一个标签显示两个变量是这么搞的 不好意思丢人了
	//arg()是QString类中的一个静态函数，使用它就可以在字符串中使用变量了。所以就不用那个强制类型转换了
	QPixmap p = this->grab(this->geometry());
	QImage image = p.toImage();
	QColor pixcolor = image.pixelColor(QPoint(event->x(), event->y()));
	qDebug() << "R" << QString::number(pixcolor.red());
	qDebug() << "G" << QString::number(pixcolor.green());
	qDebug() << "B" << QString::number(pixcolor.blue());
#endif
}

#if 0
//关于raw input 请查看msdn https://msdn.microsoft.com/en-us/library/windows/desktop/ms645536%28v=vs.85%29.aspx//效率 比 键盘钩子高太多 ，但是BCB 中无法识别 RAWINPUTDEVICE  结构 和RAW相关的函数// rawinput4.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "rawinput4.h"
#include <set>
#include <string>
#include <algorithm>


using namespace std;

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
TCHAR szTitle[MAX_LOADSTRING];                    // 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
set<DWORD> keys;

//避免按住不放不停的响应keydown
bool IsInKeys(DWORD key)
{
	set<DWORD>::iterator itor =
		find(keys.begin(), keys.end(), key);

	bool result = false;
	if (itor == keys.end())
	{
		keys.insert(key);
		result = false;
	}
	else
	{
		result = true;
	}
	return result;
}
//松开的时候，将key从set中移除
void erasekey(DWORD key)
{
	keys.erase(key);
}


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


void RegKeyboardRawInput(HWND hwnd)
{
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x06;
	Rid[0].dwFlags = RIDEV_INPUTSINK;//RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[0].hwndTarget = hwnd;

	if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE) {
		MessageBox(NULL, L"注册raw input 失败！", L"注册 raw input", MB_OK + MB_ICONINFORMATION);
	}

}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAWINPUT4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAWINPUT4));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAWINPUT4));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_RAWINPUT4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	RegKeyboardRawInput(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND    - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY    - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_INPUT:
	{
		UINT dwSize;

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
			sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL)
		{
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
			sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData doesn't return correct size !\n"));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{


			if (raw->data.keyboard.Message == WM_KEYDOWN)
			{
				if (!IsInKeys(raw->data.keyboard.VKey))
				{
					char keytext[10] = { 0 };
					BYTE state[256] = { 0 };

					//通过虚拟键盘码得到名字
					ToAscii(raw->data.keyboard.VKey, raw->data.keyboard.MakeCode, state, (LPWORD)keytext, 0);

					char buf[10] = { 0 };

					string s = "键盘按下 虚键码:" + string(keytext);

					MessageBoxA(NULL, s.c_str(), "键盘", MB_OK);
				}

			}

			if (raw->data.keyboard.Message == WM_KEYUP)
			{
				erasekey(raw->data.keyboard.VKey);
			}
		}


		delete[] lpb;
		return 0;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
#endif