#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

class Worker : public QObject
{
    Q_OBJECT
private:
	void runGrabCut(Mat src, Mat mask, Rect mRect, bool init) {
		if (mRect.width < 2 || mRect.height < 2) {
			return;
		}

		if (init) {
			grabCut(src, mask, mRect, bgModel, fgModel, 1);
		}
		else
		{
			grabCut(src, mask, mRect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
			init = true;
		}
	}
signals:
	void resultReady(Mat mask);
public slots:
    void doWork(Mat src, Mat mask, Rect mRect,bool init) {
        /* ... here is the expensive or blocking operation ... */
		qDebug() << "in mask" << &mask;
		runGrabCut(src,mask, mRect,init);
        emit resultReady(mask);
    }
private:
	Mat bgModel;
	Mat fgModel;
};