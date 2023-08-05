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

QImage BinaryzationSlow(const QImage& origin)
{
    int width = origin.width();
    int height = origin.height();
    QImage newImg = QImage(width, height, QImage::Format_RGB888);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int gray = qGray(origin.pixel(x, y));
            int newGray;
            if (gray > 128)
                newGray = 255;
            else
                newGray = 0;
            newImg.setPixel(x, y, qRgb(newGray, newGray, newGray));
        }
    }
    return newImg;
}

QImage BinaryzationFast(const QImage& img)
{
    QImage imgCopy;

    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    }
    else {
        imgCopy = QImage(img);
    }
    uint8_t* rgb = imgCopy.bits();
    int newGray = 0;
    int gray = 0;
    int size = img.width() * img.height();
    for (int i = 0; i < size; i++) {
        gray = (rgb[i * 3] + rgb[i * 3 + 1] + rgb[i * 3 + 2]) / 3;
        if (gray > 128)
            newGray = 255;
        else
            newGray = 0;
        rgb[i * 3] = newGray;
        rgb[i * 3 + 1] = newGray;
        rgb[i * 3 + 2] = newGray;
    }
    return imgCopy;
}

QImage QtOpenCV_Worker::ContourExtraction(const QImage& img)
{
    int width = img.width();
    int height = img.height();
    int pixel[8];
    QImage binImg = BinaryzationFast(img);
    QImage newImg = QImage(width, height, QImage::Format_RGB888);
    newImg.fill(Qt::white);

    uint8_t* rgb = newImg.bits();
    uint8_t* binrgb = binImg.bits();
    int nRowBytes = (width * 24 + 31) / 32 * 4;
    int  lineNum_24 = 0;
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            memset(pixel, 0, 8);
            lineNum_24 = y * nRowBytes;
            if (binrgb[lineNum_24 + x * 3] == 0) {
                rgb[lineNum_24 + x * 3] = 0;
                rgb[lineNum_24 + x * 3 + 1] = 0;
                rgb[lineNum_24 + x * 3 + 2] = 0;
                pixel[0] = binrgb[(y - 1) * nRowBytes + (x - 1) * 3];
                pixel[1] = binrgb[(y)*nRowBytes + (x - 1) * 3];
                pixel[2] = binrgb[(y + 1) * nRowBytes + (x - 1) * 3];
                pixel[3] = binrgb[(y - 1) * nRowBytes + (x) * 3];
                pixel[4] = binrgb[(y + 1) * nRowBytes + (x) * 3];
                pixel[5] = binrgb[(y - 1) * nRowBytes + (x + 1) * 3];
                pixel[6] = binrgb[(y)*nRowBytes + (x + 1) * 3];
                pixel[7] = binrgb[(y + 1) * nRowBytes + (x + 1) * 3];

                if (pixel[0] + pixel[1] + pixel[2] + pixel[3] + pixel[4] + pixel[5] + pixel[6] + pixel[7] == 0) {
                    rgb[lineNum_24 + x * 3] = 255;
                    rgb[lineNum_24 + x * 3 + 1] = 255;
                    rgb[lineNum_24 + x * 3 + 2] = 255;
                }

            }
        }
    }
    return newImg;
}
//都把QImage转化为rgb888更好运算
QImage QtOpenCV_Worker::LaplaceSharpen(const QImage& img)
{
    QImage imgCopy;
    int width = img.width();
    int height = img.height();
    int window[3][3] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(width, height, QImage::Format_RGB888);
    }
    else {
        imgCopy = QImage(img);
    }
    QImage imgCopyrgbImg = QImage(img).convertToFormat(QImage::Format_RGB888);
    uint8_t* rgbImg = imgCopyrgbImg.bits();
    uint8_t* rgb = imgCopy.bits();

    int nRowBytes = (width * 24 + 31) / 32 * 4;
    int  lineNum_24 = 0;
    for (int x = 1; x < img.width(); x++) {
        for (int y = 1; y < img.height(); y++) {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;



            for (int m = x - 1; m <= x + 1; m++)
                for (int n = y - 1; n <= y + 1; n++) {
                    if (m >= 0 && m < width && n < height) {
                        lineNum_24 = n * nRowBytes;
                        sumR += rgbImg[lineNum_24 + m * 3] * window[n - y + 1][m - x + 1];
                        sumG += rgbImg[lineNum_24 + m * 3 + 1] * window[n - y + 1][m - x + 1];
                        sumB += rgbImg[lineNum_24 + m * 3 + 2] * window[n - y + 1][m - x + 1];
                    }
                }


            int old_r = rgbImg[lineNum_24 + x * 3];
            sumR += old_r;
            sumR = qBound(0, sumR, 255);

            int old_g = rgbImg[lineNum_24 + x * 3 + 1];
            sumG += old_g;
            sumG = qBound(0, sumG, 255);

            int old_b = rgbImg[lineNum_24 + x * 3 + 2];
            sumB += old_b;
            sumB = qBound(0, sumB, 255);
            lineNum_24 = y * nRowBytes;
            rgb[lineNum_24 + x * 3] = sumR;
            rgb[lineNum_24 + x * 3 + 1] = sumG;
            rgb[lineNum_24 + x * 3 + 2] = sumB;
        }
    }

    return imgCopy;
}
