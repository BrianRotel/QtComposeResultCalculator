#include "stdafx.h"
#include "QtMyGraphicsView.h"
#include <QElapsedTimer>

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

	Mat result, binMask, outMask;
	if (mMask.empty())
	{
		return QImage();
	}
	binMask.create(mMask.size(), CV_8UC1);
	//outMask.create(256,256, CV_8UC1);
	binMask = mMask & 1;
	//outMask.setTo(GC_FGD);

	//outMask = 0 | binMask;
#if 1

	compare(mMask, GC_PR_FGD, outMask, CMP_EQ);  //取得可能标记为“可能属于前景”的像素
	//std::string title = "mMask";
	//imshow(title, mMask);
	//title = "outMask";
	//imshow(title, outMask);
	Mat foreground(Mat_Image.size(), CV_8UC3, Scalar(255, 255, 255));
	//Mat_Image.copyTo(foreground, outMask);  //不复制背景像素
	//title = "foreground";
	//imshow(title, foreground);
#endif 
#if 0
	std::string title = "mMask";
	imshow(title, mMask);//
	imwrite("mMask.png", mMask);
	title = "binMask";
	imshow(title, binMask);
	imwrite("binMask.png", binMask);
	title = "outMask";
	imshow(title, outMask);
	imwrite("outMask.png", outMask);
#endif 


#if 0
	for (size_t i = 0; i < mRect.width; i++)
	{
		QStringList l;
		for (size_t j = 0; j < mRect.height; j++)
		{
			//qDebug() << format(mMask, cv::Formatter::FMT_DEFAULT);
			l.append(QString::number(static_cast<int>(mMask.at<uchar>(i + mRect.x, j + mRect.y))));
		}
		qDebug() << l ;
	}
	qDebug() << "--------------------------------------------------------------------";
	for (size_t i = 0; i < mRect.width; i++)
	{
		QStringList l;
		for (size_t j = 0; j < mRect.height; j++)
		{
			//qDebug() << format(mMask, cv::Formatter::FMT_DEFAULT);
			l.append(QString::number(static_cast<int>(binMask.at<uchar>(i + mRect.x, j + mRect.y))));
		}
		qDebug() << l;
	}
#endif 

#if 1
	//[1] 选取区域扩大2倍,减少遍历图像的数量(原先截取区域遍历不全)
	Rect nRect;
	int scale_ratio = 2;
	int xmin = mRect.x;
	int xmax = mRect.x + mRect.width;
	int ymin = mRect.y;
	int ymax = mRect.y + mRect.height;

	int x = (xmin + xmax) / 2;
	int y = (ymin + ymax) / 2;
	int w = (xmax - xmin) * scale_ratio;
	int h = (ymax - ymin) * scale_ratio;
	//# new xmin, ymin, xmax and ymax
	xmin = x - w / 2;
	xmax = x + w / 2;
	ymin = y - h / 2;
	ymax = y + h / 2;
	// 大小修正
	xmin = max(0, int(xmin));
	ymin = max(0, int(ymin));
	xmax = min(Mat_Image.rows, int(xmax));
	ymax = min(Mat_Image.cols, int(ymax));

	nRect.x = xmin;
	nRect.y = ymin;
	nRect.width = xmax - xmin;
	nRect.height = ymax - ymin;
	//[1]
	//时间计算 选取区域计算比全图遍历节省70-80%的时间,用矩阵应该会更快  小图标截取大约耗时 1~2ms
	QElapsedTimer e;
	e.start();

	//计算最小图像位置
	int mW = 0;
	int mH = 0;
	int mX = 0;
	int mY = 0;
	bool isF = true;
	for (size_t i = 0; i < nRect.width; i++)
	{
		int sH = 0;
		bool havO = false;
		for (size_t j = 0; j < nRect.height; j++)
		{
			//qDebug() << format(mMask, cv::Formatter::FMT_DEFAULT);
			if (static_cast<int>(outMask.at<uchar>(i + nRect.x, j + nRect.y)))
			{
				++sH;
				mH = std::max(mH, sH);
				if (isF)
				{
					mX = static_cast<int>(i + nRect.x);
					mY = static_cast<int>(j + nRect.y);
					isF = false;
				}
				mX = std::min(mX, static_cast<int>(i + nRect.x));
				mY = std::min(mY, static_cast<int>(j + nRect.y));
				if (!havO)
				{
					havO = true;
				}
			}
		}
		if (havO)
		{
			mW++;
		}
	}

	//qDebug() << "mW" << mW;
	//qDebug() << "mH" << mH;
	//qDebug() << "mRectW" << mRect.width;
	//qDebug() << "mRectH" << mRect.height;
	//qDebug() << "nRectW" << nRect.width;
	//qDebug() << "nRectH" << nRect.height;
	//qDebug() << "mRecttl" << mRect.tl().x << mRect.tl().y;
	//qDebug() << "mRectbr" << mRect.br().x << mRect.br().y;
	//qDebug() << "nRecttl" << nRect.tl().x << nRect.tl().y;
	//qDebug() << "nRectbr" << nRect.br().x << nRect.br().y;
	//qDebug() << "time" << (double)e.nsecsElapsed() / (double)1000000;
#endif // 0


	//Mat_Image.copyTo(result, binMask);
	Mat_Image.copyTo(foreground, outMask);

#if 1

	Rect gRect;
	gRect.x = mY;
	gRect.y = mX;
	gRect.width = mH;
	gRect.height = mW;

	Mat cropped_image = foreground(gRect);
	//display image
	//imshow("foreground", foreground);
	//imshow("cropped_image", cropped_image);
	//imwrite("cropped.png", cropped_image);
#endif 


	return MatToImage(cropped_image);
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
		const uchar* pSrc = (const uchar*)m.data;
		QImage image(pSrc, m.cols, m.rows, m.step, QImage::Format_RGB888);

		return image.rgbSwapped();
		//cvtColor(m,m,COLOR_BGR2RGB); BGR转RGB
		//QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 3, QImage::Format_RGB888);
		//return img.rgbSwapped(); //opencv是BGR  Qt默认是RGB  所以RGB顺序转换
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
QImage QtMyGraphicsView::cvMatToQImage(const cv::Mat& mat)
{
	switch (mat.type()) {
	case CV_8UC1: {
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);

		image.setColorCount(256);
		for (int i = 0; i < 256; i++) {
			image.setColor(i, qRgb(i, i, i));
		}

		uchar* pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++) {
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}

		return image;
	}
				break;
	case CV_8UC3: {
		const uchar* pSrc = (const uchar*)mat.data;
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);

		return image.rgbSwapped();
	}
				break;
	case CV_8UC4: {
		const uchar* pSrc = (const uchar*)mat.data;
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);

		return image.copy();
	}
				break;
	default:
		break;
	}

	return QImage();
}
cv::Mat QtMyGraphicsView::QImageTocvMat(const QImage& image)
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_Grayscale8: //灰度图，每个像素点1个字节（8位）
	case QImage::Format_Indexed8: //Mat构造：行数，列数，存储结构，数据，step每行多少字节
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888: //RR,GG,BB字节顺序存储
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR); //opencv需要转为BGR的字节顺序
		break;
	}
	return mat;
}

