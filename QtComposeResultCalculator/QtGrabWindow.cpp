#include "stdafx.h"
#include "QtGrabWindow.h"

using namespace cv;

//Windows系统中使用DXGI截取桌面图像 此代码中DXGI获取图像来源于 https://www.cnblogs.com/TechNomad/p/17428347.html 
//Windows系统中使用GDI截取桌面图像
//https://maimai.cn/article/detail?fid=1745138640&efid=LSyZuQed3d1ClZ4gIMNPzQ

QtGrabWindow::QtGrabWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(QStringLiteral("Qt之grabWindow实现截图功能"));


#if 0
	//Mat image1 = imread("/test1.png", IMREAD_GRAYSCALE);
	//Mat image2 = imread("/test2.png", IMREAD_GRAYSCALE);
	//qDebug() << QDir::currentPath();
	Mat templateImg = imread("./test2.png");
	Mat srcImg = imread("./test1.png");
	//Mat dst = srcImg.clone();
	imshow("temp", templateImg);

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
	//匹配结果的四个顶点
	Point pt1(maxLoc.x, maxLoc.y);
	Point pt2(maxLoc.x + templateImg.cols, maxLoc.y);
	Point pt3(maxLoc.x, maxLoc.y + templateImg.rows);
	Point pt4(maxLoc.x + templateImg.cols, maxLoc.y + templateImg.rows);

	//画线
	line(srcImg, pt1, pt2, cv::Scalar(0, 255, 0), 11, 1);
	line(srcImg, pt2, pt4, cv::Scalar(0, 255, 0), 11, 1);
	line(srcImg, pt4, pt3, cv::Scalar(0, 255, 0), 11, 1);
	line(srcImg, pt3, pt1, cv::Scalar(0, 255, 0), 11, 1);
	//rectangle(srcImg, maxLoc, Point(maxLoc.x + templateImg.cols, maxLoc.y + templateImg.rows), Scalar(0, 255, 0), 2, 8);
	//imshow("dst", dst);
	imshow("result", srcImg);
	waitKey();
#endif
#if 0
	Mat image = imread("./test1.png");  //读取原图
	Mat templ = imread("./test2.png", 0);  //读取模板图的灰度图像

	Mat src;
	cvtColor(image, src, COLOR_RGB2GRAY); //将原图转换为灰度图像

	Mat result(src.rows - templ.rows + 1, src.cols - templ.cols + 1, CV_32FC1); //构建结果矩阵
	matchTemplate(src, templ, result, TM_CCOEFF_NORMED); //模板匹配

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
	imwrite("img.jpg", image);
	waitKey();

#endif // 0

	m_pScreen = QApplication::primaryScreen();
	startTimer(10);   //1秒25帧
	//drawOnce();
}

QtGrabWindow::~QtGrabWindow()
{}
void QtGrabWindow::paintEvent(QPaintEvent * e)
{
#if 1
    LPCWSTR className = TEXT("UnityWndClass");//UnrealWindow UnityWndClass
    LPCWSTR winName = TEXT("IdleSpiral");//剑灵 IdleSpiral
	HWND hwnd = FindWindow(className, winName);
	// 截取全屏, 指定窗口Id进行截屏
	// QPixmap pix = m_pScreen->grabWindow(QApplication::desktop()->winId());
	//QList<QScreen*> wList = qApp->screens();
	//qDebug() << wList.size();

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


	//QPixmap pix = qApp->screens()[0]->grabWindow(0);
	//QPixmap pix = m_pScreen->grabWindow((WId)hwnd);
	//QPixmap pix = m_pScreen->grabWindow(this->winId());
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