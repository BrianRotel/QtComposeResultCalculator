#include "stdafx.h"
#include "QtTestWidget.h"
#include <QPainter>

QtTestWidget::QtTestWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//图像边缘检测 -->自动抠图 
	//重写鼠标移动事件,当鼠标在想要被扣的图标上的时候,调用封装好的opencv函数,进行鼠标区域的边缘检测,然后绘制出区域,再重写按键或者点击事件,将被抠图区域保存下来,最后存储到数据库中(由用户决定)
#if 1
	Mat_Image = imread("C:/Users/Administrator/Pictures/7fe3e008f1d04ba23003a550b777cff.png");
	//Mat_Image = imread("C:/Users/Administrator/Pictures/20230805090436.png", IMREAD_GRAYSCALE);
	Qt_Image.load("C:/Users/Administrator/Pictures/7fe3e008f1d04ba23003a550b777cff.png");
	//创建一个背景遮罩
	mMask = Mat::zeros(Mat_Image.size(), CV_8UC1);
	mMask.setTo(Scalar::all(GC_BGD));
	convert2Sence(Mat_Image);
#endif
	resize(Qt_Image.size());
	//this->centralWidget()->setMouseTracking(true);
	setMouseTracking(true);
	ui.graphicsView->setScene(&scene);
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

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat img_dilate, img_erode;
	dilate(Mat_Image, img_dilate, kernel, Point(-1, -1), 3);
	erode(Mat_Image, img_erode, kernel, Point(-1, -1), 3);

	Mat laplacian;
	Laplacian(Mat_Image, laplacian, CV_8U, 3);
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
}
void QtTestWidget::mouseMoveEvent(QMouseEvent* event)
{
	mRect = Rect(Point(mRect.x, mRect.y), Point(event->pos().x(), event->pos().y()));
	qDebug() << "mouseMoveEvent:" << mRect.width << "|" << mRect.height;
	showImage();
}
void QtTestWidget::mousePressEvent(QMouseEvent* event) {
	grabMouse();
	if (event->button() == Qt::LeftButton) {//鼠标左键
		mRect.x = event->pos().x();
		mRect.y = event->pos().y();
		mRect.width = 1;
		mRect.height = 1;
		init = false;
		numRun = 0;
		qDebug() << "mousePressEvent:" << event->pos().x() << "|" << event->pos().y();
	}
}
void QtTestWidget::mouseReleaseEvent(QMouseEvent* event) {
	releaseMouse();
	if (event->button() == Qt::LeftButton) {//鼠标左键
		if (mRect.width > 1 && mRect.height > 1) {
			setROIMask();
			qDebug() << "mouseReleaseEvent:" << mRect.width << "|" << mRect.height;
			//执行grabcut的代码
			runGrabCut();
			numRun++;
			showImage();
		}

	}
}
void QtTestWidget::dropEvent(QDropEvent* event) {
	QString filePath = event->mimeData()->urls().at(0).toLocalFile();
	showCrabCutMatting(filePath.toStdString().c_str());
}

void QtTestWidget::showCrabCutMatting(const char* filePath) {
	Mat_Image = imread(filePath);
	if (Mat_Image.empty()) {
		qDebug() << "输入图像为空";
		return;
	}

	//创建一个背景遮罩
	mMask = Mat::zeros(Mat_Image.size(), CV_8UC1);
	mMask.setTo(Scalar::all(GC_BGD));
	convert2Sence(Mat_Image);
}

QImage QtTestWidget::MatToImage(Mat& m) //Mat转QImage
{
	//判断m的类型，可能是CV_8UC1  CV_8UC2  CV_8UC3  CV_8UC4
	switch (m.type())
	{ //QIamge 构造函数, ((const uchar *data, 宽(列),高(行), 一行共多少个（字节）通道，宽度*字节数，宏参数)
	case CV_8UC1:
	{
		QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 1, QImage::Format_Grayscale8);
		return img;
	}
	break;
	case CV_8UC3:   //一个像素点由三个字节组成
	{
		//cvtColor(m,m,COLOR_BGR2RGB); BGR转RGB
		QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 3, QImage::Format_RGB888);
		return img.rgbSwapped(); //opencv是BGR  Qt默认是RGB  所以RGB顺序转换
	}
	break;
	case CV_8UC4:
	{
		QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 4, QImage::Format_RGBA8888);
		return img;
	}
	break;
	default:
	{
		QImage img; //如果遇到一个图片均不属于这三种，返回一个空的图片
		return img;
	}
	}
}
Mat QtTestWidget::ImageToMat(QImage& image) //QImage转Mat
{
	Mat mat = Mat::zeros(image.height(), image.width(), image.format()); //初始化Mat
	switch (image.format()) //判断image的类型
	{
	case QImage::QImage::Format_Grayscale8:  //灰度图
		mat = Mat(image.height(), image.width(),
			CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888: //3通道彩色
		mat = Mat(image.height(), image.width(),
			CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_ARGB32: //4通道彩色
		mat = Mat(image.height(), image.width(),
			CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	default:
		return mat;
	}
	return mat;
}
void QtTestWidget::convert2Sence(Mat target) {
	scene.clear();
	if (target.empty())
	{
		return;
	}
	QImage image = MatToImage(target);
	if (image.isNull())
	{
		return;
	}
	QPixmap pixmap = QPixmap::fromImage(image);
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap.scaled(ui.graphicsView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	scene.addItem(item);
}
/**
 * 将选中的区域设置为前景
 * @brief CrabCut_Matting::setROIMask
 */
void QtTestWidget::setROIMask() {
	// GC_FGD = 1
	// GC_BGD =0;
	// GC_PR_FGD = 3
	// GC_PR_BGD = 2
	mMask.setTo(GC_BGD);
	mRect.x = max(0, mRect.x);
	mRect.y = max(0, mRect.y);
	mRect.width = min(mRect.width, Mat_Image.cols - mRect.x);
	mRect.height = min(mRect.height, Mat_Image.rows - mRect.y);
	mMask(mRect).setTo(Scalar(GC_PR_FGD));//将选中的区域设置为
}

void QtTestWidget::showImage() {
	Mat result, binMask;
	if (mMask.empty())
	{
		return;
	}
	binMask.create(mMask.size(), CV_8UC1);
	binMask = mMask & 1;
	if (init) {
		Mat_Image.copyTo(result, binMask);
	}
	else {
		Mat_Image.copyTo(result);
	}
	rectangle(result, mRect, Scalar(0, 0, 255), 2, 8);
	convert2Sence(result);

}


void QtTestWidget::runGrabCut() {
	if (mRect.width < 2 || mRect.height < 2) {
		return;
	}

	if (init) {
		grabCut(Mat_Image, mMask, mRect, bgModel, fgModel, 1);
	}
	else 
	{
		grabCut(Mat_Image, mMask, mRect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
		init = true;
	}
}
void QtTestWidget::paintEvent(QPaintEvent* event)
{
	//Q_UNUSED(event)
	//QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing);
	//painter.drawPixmap(this->rect(), Qt_Image);
}











