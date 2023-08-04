#pragma once

#include <QObject>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
using namespace cv;

class QtOpenCV_Worker  : public QObject
{
	Q_OBJECT

public:
	QtOpenCV_Worker(QObject *parent);
	~QtOpenCV_Worker();
    void laplaceGrad(cv::Mat& src, cv::Mat& dst);
signals:
    void threadId(int*); //这个信号用来返回线程id标识
public slots:
    //槽函数，子线程任务在这个函数中处理
    void task(cv::Mat* src, cv::Mat* dst, uchar flag);
};


