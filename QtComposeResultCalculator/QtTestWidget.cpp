#include "stdafx.h"
#include "QtTestWidget.h"
#include <QPainter>
#include <QElapsedTimer>
//grabcut算法
bool setMouse = false;    //判断鼠标左键的状态（up / down）
bool init;
Point pt;
Rect rect;
Mat srcImg, gMask, bgModel, fgModel;
int numRun = 0;
void onMouse(int, int, int, int, void*);
void runGrabCut();
void showImage();
QtTestWidget::QtTestWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//图像边缘检测 -->自动抠图 
	//重写鼠标移动事件,当鼠标在想要被扣的图标上的时候,调用封装好的opencv函数,进行鼠标区域的边缘检测,然后绘制出区域,再重写按键或者点击事件,将被抠图区域保存下来,最后存储到数据库中(由用户决定)
#if 1
	Qt_Image.load("C:/Users/Administrator/Pictures/7fe3e008f1d04ba23003a550b777cff.png");
#endif
	resize(Qt_Image.size());
	setMouseTracking(true);
	myView = new QtMyGraphicsView(this);
	ui.verticalLayout->insertWidget(0, myView);

	bool isConnect = QObject::connect(ui.pushButton, SIGNAL(clicked(bool)), this, SLOT(onPushButton(bool)));
	isConnect = QObject::connect(ui.pushButton_2, SIGNAL(clicked(bool)), this, SLOT(onPushButton2(bool)));
	//doSomeThing();//边缘检测
	//QMessageBox::warning(this, "源图像", "");
	//doSomeThing2();//区域选图检测
}

QtTestWidget::~QtTestWidget()
{

}

void QtTestWidget::Transfer()
{
	// QPixmap 读 图像
	QPixmap map("/path/to/image.png");

	// or
	// QPixMap加载 图像
	map.load("/other.png");
	// QPixmap 存储图像
	map.save("/save.png");

	// QPixmap ---》 QImage 的转换
	QImage qimg = map.toImage();

	// QImage ---->QPixmap 转换
	QPixmap map2 = QPixmap::fromImage(qimg);


	cv::Mat mat = cv::imread("/path/to/an/img.png");
	cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

	//Mat ---> QImage的转换
	QImage  img2(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);

	// QImage --->Mat的转换
	cv::Mat mat2 = cv::Mat(img2.height(), img2.width(), CV_8UC3, img2.bits(), img2.bytesPerLine());
}
void QtTestWidget::doSomeThing()
{
	//    get_outline(image);
	Mat mImage = imread("C:/Users/Administrator/Pictures/7fe3e008f1d04ba23003a550b777cff.png", IMREAD_GRAYSCALE);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat img_dilate, img_erode;
	dilate(mImage, img_dilate, kernel, Point(-1, -1), 3);
	erode(mImage, img_erode, kernel, Point(-1, -1), 3);

	Mat laplacian;
	Laplacian(mImage, laplacian, CV_8U, 3);
	threshold(laplacian, laplacian, 127, 255, THRESH_BINARY | THRESH_OTSU);

	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	findContours(laplacian, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	qDebug() << hierarchy.size();

	//    Mat draw_img = Mat::zeros(image.size(), CV_8UC3);
	Mat image1 = imread("C:/Users/Administrator/Pictures/7fe3e008f1d04ba23003a550b777cff.png", IMREAD_COLOR);
	drawContours(image1, contours, -1, Scalar(0, 0, 255), 1);
	imshow("Photo Boundaries", image1);

	waitKey(0);
}
void QtTestWidget::doSomeThing2()
{
	srcImg = imread("C:/Users/Administrator/Pictures/7fe3e008f1d04ba23003a550b777cff.png");
	if (srcImg.empty())
	{
		printf("could not load image...\n");
	}
	std::string title = "src";
	imshow(title, srcImg);

	gMask.create(srcImg.size(), CV_8U);
	setMouseCallback(title, onMouse, 0);

	while (1)
	{
		char c = (char)waitKey(0);
		if (c == ' ') {//选中矩形框后，按空格键执行grabcut分割
			runGrabCut();
			numRun++;
			showImage();
			printf("current iteative times : %d\n", numRun);
		}
		if ((int)c == 27) {
			break;
		}
		//if (getWindowProperty(title, 0) == -1)
		//{
		//	break;
		//}
	}
	destroyAllWindows();
}

void showImage()
{
	Mat result, binmask;
	binmask = gMask & 1;				//进一步掩膜
	if (init)						//进一步抠出无效区域。鼠标按下，init变为false
	{
		srcImg.copyTo(result, binmask);
	}
	else
	{
		result = srcImg.clone();
	}

	rectangle(result, rect, Scalar(0, 0, 255), 2, 8);
	std::string title = "src";
	imshow(title, result);
}

void onMouse(int events, int x, int y, int flag, void*)
{
	if (x < 0 || y < 0 || x > srcImg.cols || y > srcImg.rows)	//无效区域
		return;


	if (events == EVENT_LBUTTONDOWN)
	{
		setMouse = true;
		pt.x = x;
		pt.y = y;
		init = false;
	}
	else if (events == EVENT_MOUSEMOVE)//鼠标只要动，就执行一次
	{
		if (setMouse == true)			//鼠标左键按住，滑动
		{
			Point pt1;
			pt1.x = x;
			pt1.y = y;
			rect = Rect(pt, pt1);//定义矩形区域
			showImage();
			gMask.setTo(Scalar::all(GC_BGD));//背景
			gMask(rect).setTo(Scalar(GC_PR_FGD));//前景			    //对rect内部设置为可能的前景，外部设置为背景
		}
	}
	else if (events == EVENT_LBUTTONUP)
		setMouse = false;	        	//鼠标左键抬起
}

void runGrabCut()
{
	if (init)//鼠标按下，init变为false
		grabCut(srcImg, gMask, rect, bgModel, fgModel, 1);//第二次迭代，用gMask初始化grabcut
	else
	{
		grabCut(srcImg, gMask, rect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);//用矩形窗初始化GrabCut
		init = true;
	}
}
void QtTestWidget::onPushButton(bool b)
{
	myClip.show();
	QElapsedTimer e;
	e.start();
	QImage img = myView->getGrabImage();
	qDebug() << "GrabImageTime" << e.elapsed();
	myClip.setImage(img);
}
void QtTestWidget::onPushButton2(bool b)
{

}

#ifdef SHOW_BACKGROUND
void QtTestWidget::paintEvent(QPaintEvent* event)
{
	//Q_UNUSED(event)
	//QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing);
	//painter.drawPixmap(this->rect(), Qt_Image);
}
#endif











