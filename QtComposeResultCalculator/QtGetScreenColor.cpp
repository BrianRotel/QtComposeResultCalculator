#include "stdafx.h"
#include "QtGetScreenColor.h"
//需要添加的头文件
#include <QPixmap>
#include <QImage>
#include <QApplication>
#include <QWindow>
#include <QScreen>

QtGetScreenColor::QtGetScreenColor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    timer = new QTimer(this);

    //QTimer::singleShot(1000, this, &QtGetScreenColor::showColorValue);
    bool isConnect = connect(timer, &QTimer::timeout, this, &QtGetScreenColor::showColorValue);
    timer->start(10);
}

QtGetScreenColor::~QtGetScreenColor()
{}


//定时器内操作的函数
void QtGetScreenColor::showColorValue()
{
    //获取鼠标x，y坐标
    int x = QCursor::pos().x();
    int y = QCursor::pos().y();

    //输出坐标
    ui.lineEditCoordinate->setText(tr("x:%1  y:%2").arg(x).arg(y));

    //获取坐标像素点
    QScreen* screen = QGuiApplication::primaryScreen();
    if (const QWindow* window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;
    QWindow window;
    QPixmap pixmap = screen->grabWindow(0);

    //获取像素点RGB
    int red, green, blue;
    if (!pixmap.isNull())
    {
        QImage image = pixmap.toImage();

        if (!image.isNull())
        {
            QColor color = image.pixel(x, y);
            red = color.red();
            green = color.green();
            blue = color.blue();

            //输出16进制的颜色
            QString hRed = QString::number(red, 16).toUpper();
            QString hGreen = QString::number(green, 16).toUpper();
            QString hBlue = QString::number(blue, 16).toUpper();
            ui.lineEditHex->setText(tr("#%1%2%3").arg(hRed).arg(hGreen).arg(hBlue));

            //输出RGB
            ui.lineEditRGB->setText(tr("R:%1 G:%2 B:%3").arg(red).arg(green).arg(blue));

            //设置label标签的颜色显示
            int width = ui.labelColorDisplay->width();
            int height = ui.labelColorDisplay->height();
            QPixmap labelPix(width, height);
            labelPix.fill(color);
            ui.labelColorDisplay->setPixmap(labelPix);
        }
    }

}
