#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

static QImage MatToImage(Mat& m) //Mat转QImage
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

static Mat ImageToMat(QImage& image) //QImage转Mat
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
static QImage cvMatToQImage(const cv::Mat& mat)
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
static cv::Mat QImageTocvMat(const QImage& image)
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

static QImage cvMat2QImage(const cv::Mat& mat)
{
    if (mat.empty())
    {
        return QImage();
    }
    QImage image;
    switch (mat.type())
    {
    case CV_8UC1:
    {
        image = QImage((const uchar*)(mat.data),
            mat.cols, mat.rows, mat.step,
            QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8UC2:
    {
        mat.convertTo(mat, CV_8UC1);
        image = QImage((const uchar*)(mat.data),
            mat.cols, mat.rows, mat.step,
            QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8UC3:
    {
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC4:
    {
        //cv::cvtColor(mat, mat, cv::COLOR_BGRA2RGBA);
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    case CV_32FC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar* pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_32FC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar* pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_64FC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar* pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_64FC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar* pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_32SC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar* pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_32SC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar* pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_16SC1:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar* pSrc = (const uchar*)normalize_mat.data;
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_16SC3:
    {
        Mat normalize_mat;
        normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        normalize_mat.convertTo(normalize_mat, CV_8U);
        const uchar* pSrc = (const uchar*)normalize_mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, normalize_mat.cols, normalize_mat.rows, normalize_mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8SC1:
    {
        //Mat normalize_mat;
        //normalize(mat, normalize_mat, 0, 255, NORM_MINMAX, -1);
        mat.convertTo(mat, CV_8U);
        const uchar* pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    case CV_8SC3:
    {
        mat.convertTo(mat, CV_8U);
        const uchar* pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    default:
        mat.convertTo(mat, CV_8UC3);
        QImage image((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
        return QImage();
        break;
    }
}
static cv::Mat QImage2cvMat(const QImage& image)
{
    cv::Mat mat;
    //qDebug() << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        //cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA);
        break;
    case QImage::Format_RGB32:
    {
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::Mat mat2;
        cv::cvtColor(mat, mat2, cv::COLOR_BGRA2BGR); //4通道转换为3通道,仅仅把通道A去掉,BGR不变
        mat = mat2.clone();
        //cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA);
        //cv::cvtColor(mat, mat, CV_BGR2RGB);
    }
        break;
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        //cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_Grayscale8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}