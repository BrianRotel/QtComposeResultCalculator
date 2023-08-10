#pragma once
#include <QWidget>
#include "ui_QtTestWidget.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

class QtTestWidget : public QWidget
{
	Q_OBJECT

public:
	QtTestWidget(QWidget *parent = nullptr);
	~QtTestWidget();

	void Transfer();
	void doSomeThing();

protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void dropEvent(QDropEvent* event);
	void showCrabCutMatting(const char* filePath);
	QImage MatToImage(Mat& m);
	Mat ImageToMat(QImage& image);
	void convert2Sence(Mat target);
	void setROIMask();
	void showImage();
	void runGrabCut();
	void paintEvent(QPaintEvent* event) override;
private:
	Ui::QtTestWidgetClass ui;
private:
	Mat Mat_Image;
	Mat mMask;
	Mat bgModel;
	Mat fgModel;
	Rect mRect;
	int numRun;
	QPixmap Qt_Image;
	QGraphicsScene scene;
	bool init;
};
