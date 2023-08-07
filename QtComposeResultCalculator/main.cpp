#include "stdafx.h"
#include "QtComposeResultCalculator.h"
#include "QtGetWindowInfo.h"
#include "QtGrabWindow.h"
#include <QtWidgets/QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
#if 0
	QApplication a(argc, argv);

	std::shared_ptr<QtGrabWindow> screen_capture = std::make_shared<QtGrabWindow>();
	if (!screen_capture.get()->InitD3D11Device()) {
		return  -1;
	}

	if (!screen_capture.get()->InitDuplication()) {
		return -1;
	}

	int counts = 0;
	/* 每隔1秒获取一次图像 */
	while (counts < 5) {
		QString fileName = QString("%1.png").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
		screen_capture.get()->GetDesktopFrame(fileName);

		counts++;
		Sleep(1000);
	}
#endif
    QtGrabWindow c;
    c.show();
    QtGetWindowInfo b;
    b.show();
    QtComposeResultCalculator w;
    w.show();
    return a.exec();
}
