#include "stdafx.h"
#include "QtComposeResultCalculator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtComposeResultCalculator w;
    w.show();
    return a.exec();
}
