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
protected:

	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void setROIMask();
	void showImage();
	void runGrabCut();
	void convert2Sence(Mat target);

	QImage MatToImage(Mat& m);
	Mat ImageToMat(QImage& image);
#ifdef DROP_IMAGE
	void dropEvent(QDropEvent* event);
	void showCrabCutMatting(const char* filePath);
#endif
private:
	Ui::QtMyGraphicsViewClass ui;

	int numRun;
	bool init;
	bool isPress;
	Rect mRect;
	Mat mMask;
	Mat Mat_Image;
	Mat bgModel;
	Mat fgModel;
	QGraphicsScene scene;
};
