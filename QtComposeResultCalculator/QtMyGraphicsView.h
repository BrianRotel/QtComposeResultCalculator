#pragma once

#include <QGraphicsView>
#include "ui_QtMyGraphicsView.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

class QtMyGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	QtMyGraphicsView(QWidget *parent = nullptr);
	~QtMyGraphicsView();

	inline size_t getSourceImgHeight() { return Mat_Image.rows; }
	inline size_t getSourceImgWidth() { return Mat_Image.cols; }
protected:

	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void setROIMask();
	void showImage();
	QImage getImage();
	cv::Rect expandRect(float multiple);
	void convert2Sence(Mat target);
	void runGrabCut();

	//QImage MatToImage(Mat& m);
	//QImage cvMatToQImage(const cv::Mat& mat);
	//cv::Mat QImageTocvMat(const QImage& image);
	//Mat ImageToMat(QImage& image);
#ifdef DROP_IMAGE
	void dropEvent(QDropEvent* event);
	void showCrabCutMatting(const char* filePath);
#endif
public:
	void getGrabImage();
public slots:	 
	void handleResults(Mat);
signals:
	void operate(Mat src, Mat mask, Rect mRect, bool init);
	void returnResult(QImage result);
private:
	Ui::QtMyGraphicsViewClass ui;

	int numRun;
	bool init;
	bool isPress;
	Rect mRect;
	Mat mMask;
	Mat Mat_Image;

	QGraphicsScene scene;
	QThread workerThread;
};
