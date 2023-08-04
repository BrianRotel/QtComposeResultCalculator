#include "stdafx.h"
#include "QtOpenCV_Worker.h"
#include<QDebug>
#include<QThread>

QtOpenCV_Worker::QtOpenCV_Worker(QObject *parent)
	: QObject(parent)
{}

QtOpenCV_Worker::~QtOpenCV_Worker()
{}

//flag是一个任务类型标识，0的时候做Canny边缘检测，1的时候做拉普拉斯算子
void QtOpenCV_Worker::task(cv::Mat* src, cv::Mat* dst, uchar flag)
{
    if (src->data == NULL)
        return;
    if (flag == 0) //Canny边缘检测
    {
        cv::Canny(*src, *dst, 20, 40, 3); //3是直径，应在3~7范围内，否则会报错
        //        qDebug()<<QThread::currentThreadId();
    }
    if (flag == 1) //拉普拉斯算子，梯度的散度
    {
        this->laplaceGrad(*src, *dst);
    }
    emit threadId((int*)QThread::currentThreadId()); //当前线程id返回
}

//laplace任务
void QtOpenCV_Worker::laplaceGrad(cv::Mat& src, cv::Mat& dst)
{

    if (src.channels() != 1)
        return;

    int rows = src.rows;
    int cols = src.cols;

    dst = cv::Mat::zeros(rows, cols, CV_32FC1);

    for (int i = 1; i < rows - 1; i++)
    {
        for (int j = 1; j < cols - 1; j++)
        {
            //            [1,1,1;1,-8,1;1,1,1]
            dst.ptr<float>(i)[j] = 1.0 * src.ptr<uchar>(i - 1)[j - 1] + 1.0 * src.ptr<uchar>(i - 1)[j] + 1.0 * src.ptr<uchar>(i - 1)[j + 1]
                + 1.0 * src.ptr<uchar>(i)[j - 1] - 8.0 * src.ptr<uchar>(i)[j] + 1.0 * src.ptr<uchar>(i)[j + 1]
                + 1.0 * src.ptr<uchar>(i + 1)[j - 1] + 1.0 * src.ptr<uchar>(i + 1)[j] + 1.0 * src.ptr<uchar>(i + 1)[j + 1];
        }
    }
    cv::convertScaleAbs(dst, dst, 1, 0); // 从CV_32FC1转换到CV_8UC1
}
