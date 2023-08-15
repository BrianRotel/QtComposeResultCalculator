#include "stdafx.h"
#include "QtComposeResultCalculator.h"
#include "QtGetWindowInfo.h"
#include "QtGrabWindow.h"
#include "QtMyClipboard.h"
#include "QtTestWidget.h"
#include "QtMyGraphicsView.h"
#include "QtWatting.h"
#include "QtGetScreenColor.h"

#include <QtWidgets/QApplication>
#include <QScreen>

#include <iostream>
#include <string>
#include "DXGIDuplicator.h"

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);

#if 0
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
#endif
    QtGetWindowInfo b;
    b.show();
   // QtGrabWindow c;
   // c.show();
    //QtMyClipboard d;
    //d.show();
    //QtTestWidget e;
    //e.show();
    //QtMyGraphicsView f;
    //f.show();
    //QtGetScreenColor g;
    //g.show();
    return a.exec();
}
