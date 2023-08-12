#pragma once

#include <QObject>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include "ui_QtGrabWindow.h"
using namespace cv;
QT_BEGIN_NAMESPACE
namespace Ui { class QtOpenCV_Worker; }
QT_END_NAMESPACE
//边缘检测类,暂时被弃用
class QtOpenCV_Worker  : public QMainWindow
{
	Q_OBJECT

public:
	QtOpenCV_Worker(QWidget * parent = nullptr);
	~QtOpenCV_Worker();
    void laplaceGrad(cv::Mat& src, cv::Mat& dst);
    QImage ContourExtraction(const QImage& img);
    QImage LaplaceSharpen(const QImage& img);
signals:
    void threadId(int*); //这个信号用来返回线程id标识
public slots:
    //槽函数，子线程任务在这个函数中处理
    void task(cv::Mat* src, cv::Mat* dst, uchar flag);
private:
    Ui::QtGrabWindowClass ui;
};


