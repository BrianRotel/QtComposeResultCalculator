#include "stdafx.h"
#include "QtGetScreenColor.h"
//��Ҫ��ӵ�ͷ�ļ�
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


//��ʱ���ڲ����ĺ���
void QtGetScreenColor::showColorValue()
{
    //��ȡ���x��y����
    int x = QCursor::pos().x();
    int y = QCursor::pos().y();

    //�������
    ui.lineEditCoordinate->setText(tr("x:%1  y:%2").arg(x).arg(y));

    //��ȡ�������ص�
    QScreen* screen = QGuiApplication::primaryScreen();
    if (const QWindow* window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;
    QWindow window;
    QPixmap pixmap = screen->grabWindow(0);

    //��ȡ���ص�RGB
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

            //���16���Ƶ���ɫ
            QString hRed = QString::number(red, 16).toUpper();
            QString hGreen = QString::number(green, 16).toUpper();
            QString hBlue = QString::number(blue, 16).toUpper();
            ui.lineEditHex->setText(tr("#%1%2%3").arg(hRed).arg(hGreen).arg(hBlue));

            //���RGB
            ui.lineEditRGB->setText(tr("R:%1 G:%2 B:%3").arg(red).arg(green).arg(blue));

            //����label��ǩ����ɫ��ʾ
            int width = ui.labelColorDisplay->width();
            int height = ui.labelColorDisplay->height();
            QPixmap labelPix(width, height);
            labelPix.fill(color);
            ui.labelColorDisplay->setPixmap(labelPix);
        }
    }

}
