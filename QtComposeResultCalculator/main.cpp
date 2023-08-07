#include "stdafx.h"
#include "QtComposeResultCalculator.h"
#include "QtGetWindowInfo.h"
#include <QtWidgets/QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    //QScreen* screen = QGuiApplication::primaryScreen();
    //if (screen) {
    //    QPixmap originalPixmap = screen->grabWindow(0);
    //}

    QApplication a(argc, argv);
    QtGetWindowInfo b;
    b.show();
    QtComposeResultCalculator w;
    w.show();
    return a.exec();
}
