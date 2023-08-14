#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

QImage MatToImage(Mat& m) //MatתQImage
{
	//�ж�m�����ͣ�������CV_8UC1  CV_8UC2  CV_8UC3  CV_8UC4
	switch (m.type())
	{ //QIamge ���캯��, ((const uchar *data, ��(��),��(��), һ�й����ٸ����ֽڣ�ͨ�������*�ֽ����������)
	case CV_8UC1:
	{
		QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 1, QImage::Format_Grayscale8);
		return img;
	}
	break;
	case CV_8UC3:   //һ�����ص��������ֽ����
	{
		const uchar* pSrc = (const uchar*)m.data;
		QImage image(pSrc, m.cols, m.rows, m.step, QImage::Format_RGB888);

		return image.rgbSwapped();
		//cvtColor(m,m,COLOR_BGR2RGB); BGRתRGB
		//QImage img((uchar*)m.data, m.cols, m.rows, m.cols * 3, QImage::Format_RGB888);
		//return img.rgbSwapped(); //opencv��BGR  QtĬ����RGB  ����RGB˳��ת��
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
		QImage img; //�������һ��ͼƬ�������������֣�����һ���յ�ͼƬ
		return img;
	}
	}
}

Mat ImageToMat(QImage& image) //QImageתMat
{
	Mat mat = Mat::zeros(image.height(), image.width(), image.format()); //��ʼ��Mat
	switch (image.format()) //�ж�image������
	{
	case QImage::QImage::Format_Grayscale8:  //�Ҷ�ͼ
		mat = Mat(image.height(), image.width(),
			CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888: //3ͨ����ɫ
		mat = Mat(image.height(), image.width(),
			CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_ARGB32: //4ͨ����ɫ
		mat = Mat(image.height(), image.width(),
			CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	default:
		return mat;
	}
	return mat;
}
QImage cvMatToQImage(const cv::Mat& mat)
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
cv::Mat QImageTocvMat(const QImage& image)
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_Grayscale8: //�Ҷ�ͼ��ÿ�����ص�1���ֽڣ�8λ��
	case QImage::Format_Indexed8: //Mat���죺�������������洢�ṹ�����ݣ�stepÿ�ж����ֽ�
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888: //RR,GG,BB�ֽ�˳��洢
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR); //opencv��ҪתΪBGR���ֽ�˳��
		break;
	}
	return mat;
}