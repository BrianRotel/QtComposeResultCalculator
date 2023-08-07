#include "stdafx.h"
#include "QtComposeResultCalculator.h"
#include "QtGetWindowInfo.h"
#include "QtGrabWindow.h"
#include <QtWidgets/QApplication>
#include <QScreen>

#include <iostream>
#include <string>
#include "DXGIDuplicator.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


    QtGrabWindow c;
    if (!c.InitD3D11Device()) {
        return  -1;
    }

    if (!c.InitDuplication()) {
        return -1;
    }
    c.show();
    QtGetWindowInfo b;
    b.show();
    QtComposeResultCalculator w;
    w.show();
    return a.exec();
}
