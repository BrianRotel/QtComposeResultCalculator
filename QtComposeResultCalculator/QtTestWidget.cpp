#include "stdafx.h"
#include "QtTestWidget.h"

using namespace cv;
QtTestWidget::QtTestWidget(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//图像边缘检测 -->自动抠图 
	//重写鼠标移动事件,当鼠标在想要被扣的图标上的时候,调用封装好的opencv函数,进行鼠标区域的边缘检测,然后绘制出区域,再重写按键或者点击事件,将被抠图区域保存下来,最后存储到数据库中(由用户决定)
#if 1
	Mat image = imread("C:/Users/Administrator/Pictures/20230805090436.png", IMREAD_GRAYSCALE);
	//    get_outline(image);

	//    Mat image = imread("temp.png", IMREAD_GRAYSCALE);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat img_dilate, img_erode;
	dilate(image, img_dilate, kernel, Point(-1, -1), 3);
	erode(image, img_erode, kernel, Point(-1, -1), 3);

	Mat laplacian;
	Laplacian(image, laplacian, CV_8U, 3);
	threshold(laplacian, laplacian, 127, 255, THRESH_BINARY | THRESH_OTSU);

	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	findContours(laplacian, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	qDebug() << hierarchy.size();

	//    Mat draw_img = Mat::zeros(image.size(), CV_8UC3);
	Mat image1 = imread("C:/Users/Administrator/Pictures/20230805090436.png", IMREAD_COLOR);
	drawContours(image1, contours, -1, Scalar(0, 0, 255), 1);
	imshow("Photo Boundaries", image1);

	waitKey(0);
#endif
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

void QtTestWidget::mouseMoveEvent(QMouseEvent* event)
{

}













