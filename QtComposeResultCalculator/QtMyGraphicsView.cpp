#include "stdafx.h"
#include "QtMyGraphicsView.h"

QtMyGraphicsView::QtMyGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	ui.setupUi(this);
	setMouseTracking(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	Mat_Image = imread("C:/Users/Administrator/Pictures/7fe3e008f1d04ba23003a550b777cff.png");
	mMask = Mat::zeros(Mat_Image.size(), CV_8UC1);
	mMask.setTo(Scalar::all(GC_BGD));
	convert2Sence(Mat_Image);
	this->setScene(&scene);
	//resize(Mat_Image.cols, Mat_Image.rows);
	setFixedSize(Mat_Image.cols, Mat_Image.rows);
	init = false;
	isPress = false;
	showImage();
	//std::string title = "src";
	//imshow(title, mMask);
}

QtMyGraphicsView::~QtMyGraphicsView()
{}

void QtMyGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
	if (isPress)
	{
		mRect = Rect(Point(mRect.x, mRect.y), Point(event->pos().x(), event->pos().y()));
	}
	qDebug() << "mouseMoveEvent:" << mRect.tl().x << "," << mRect.tl().y << "|" << mRect.br().x << "," << mRect.br().y;
	showImage();
}
void QtMyGraphicsView::mousePressEvent(QMouseEvent* event) {
	//this->grabMouse(Qt::CrossCursor);
	this->setCursor(Qt::CrossCursor);
	if (event->button() == Qt::LeftButton) {//鼠标左键
		QPoint globalPos(event->globalPos());
		QPoint localPos(this->mapFromGlobal(globalPos));
		mRect.x = localPos.x();
		mRect.y = localPos.y();
		mRect.width = 1;
		mRect.height = 1;
		init = false;
		isPress = true;
		numRun = 0;
		qDebug() << "mousePressEvent:" << event->pos().x() << "|" << event->pos().y();
	}
}
void QtMyGraphicsView::mouseReleaseEvent(QMouseEvent* event) {
	//this->releaseMouse();
	this->unsetCursor();
	if (event->button() == Qt::LeftButton) {//鼠标左键
		isPress = false;
		if (mRect.width > 1 && mRect.height > 1) {
			setROIMask();
			qDebug() << "mouseReleaseEvent:" << mRect.width << "|" << mRect.height;
			//执行grabcut的代码
			//runGrabCut();
			numRun++;
			//showImage();
		}

	}
}

/**
 * 将选中的区域设置为前景
 * @brief CrabCut_Matting::setROIMask
 */
void QtMyGraphicsView::setROIMask() {
	mMask.setTo(GC_BGD);
	//std::string title = "src1";
	//imshow(title, mMask);
	mRect.x = max(0, mRect.x);
	mRect.y = max(0, mRect.y);
	mRect.width = min(mRect.width, Mat_Image.cols - mRect.x);
	mRect.height = min(mRect.height, Mat_Image.rows - mRect.y);
	mMask(mRect).setTo(Scalar(GC_PR_FGD));//将选中的区域设置为
	//title = "src2";
	//imshow(title, mMask);
}

void QtMyGraphicsView::showImage() {
	Mat result;
	if (mMask.empty())
	{
		return;
	}
	Mat_Image.copyTo(result);
	//绘制矩形,图像,矩形,颜色,线宽,样式
	if (mRect.width > 1 && mRect.height > 1) {
		rectangle(result, mRect, Scalar(0, 0, 255), 2, 8);
	}
	convert2Sence(result);

}
QImage QtMyGraphicsView::getImage()
{
	Mat result, binMask;
	if (mMask.empty())
	{
		return QImage();
	}
	binMask.create(mMask.size(), CV_8UC1);
	binMask = mMask & 1;
	Mat_Image.copyTo(result, binMask);
	return MatToImage(result);
}
void QtMyGraphicsView::convert2Sence(Mat target) {
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
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	scene.addItem(item);
}
void QtMyGraphicsView::runGrabCut() {
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
QImage QtMyGraphicsView::getGrabImage()
{
	runGrabCut();
	return getImage();
}

#ifdef DROP_IMAGE
void QtMyGraphicsView::dropEvent(QDropEvent* event) {
	QString filePath = event->mimeData()->urls().at(0).toLocalFile();
	showCrabCutMatting(filePath.toStdString().c_str());
}

void QtMyGraphicsView::showCrabCutMatting(const char* filePath) {
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
#endif

QImage QtMyGraphicsView::MatToImage(Mat& m) //Mat转QImage
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
Mat QtMyGraphicsView::ImageToMat(QImage& image) //QImage转Mat
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


