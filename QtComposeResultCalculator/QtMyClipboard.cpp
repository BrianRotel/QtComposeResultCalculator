#include "stdafx.h"
#include "QtMyClipboard.h"
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QImageReader>

QtMyClipboard::QtMyClipboard(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    Init();

}

QtMyClipboard::~QtMyClipboard()
{}


void QtMyClipboard::Init()
{
#ifdef UIWIDGET
    ui.widget->setAutoFillBackground(true);
#endif
    bool isConnect = QObject::connect(ui.pushButton_2, SIGNAL(clicked(bool)), this, SLOT(onPushButton2(bool)));
}
#ifdef UIWIDGET
void QtMyClipboard::resizeEvent(QResizeEvent* event)
{
    //多次调用后图片精度由于缩放会全部丢失,不可取 Inherits:
    QPalette palette = ui.widget->palette();
    QBrush brush = palette.brush(QPalette::Window);
    brush.setTextureImage(brush.textureImage().scaled(ui.widget->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    palette.setBrush(QPalette::Window, brush);
    ui.widget->setPalette(palette);
}


void QtMyClipboard::onPushButton1(bool b)
{
    QClipboard* clipboard = QApplication::clipboard();
    const QMimeData* mimeData = clipboard->mimeData();
    if (mimeData->hasImage()) 
    {
        //将图片数据转为QImage
        QImage img = qvariant_cast<QImage>(mimeData->imageData());
        if (!img.isNull())
        {
            //缩放会丢失精度,必须存一份原始数据,这里应该只是显示
            img = img.scaled(ui.widget->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            QPalette palette = ui.widget->palette();
            palette.setBrush(QPalette::Window, QBrush(img));
            ui.widget->setPalette(palette);
        }
    }
}
#endif
void QtMyClipboard::onPushButton2(bool b)
{
    QClipboard* clipboard = QApplication::clipboard();
    const QMimeData* mimeData = clipboard->mimeData();
    if (mimeData->hasImage())
    {
        //将图片数据转为QImage
        QImage img = qvariant_cast<QImage>(mimeData->imageData());
        if (!img.isNull())
        {
            QTextDocument* textDocument1 = ui.textEdit->document();
            textDocument1->addResource(QTextDocument::ImageResource, QUrl("name"), QVariant(img));
            ui.textEdit->textCursor().insertImage("name");
        }
    }
    else if(mimeData->hasText())
    {
        //将文本数据转为QImage
        QString str = mimeData->text();
        if (!str.isEmpty())
        {
            ui.textEdit->textCursor().insertText(str);
        }
    }
    else 
    {
        //ui.textEdit->insertFromMimeData(mimeData);
    }
}

void QtMyClipboard::setImage(QImage img)
{
    if (!img.isNull())
    {
        QTextDocument* textDocument1 = ui.textEdit->document();
        textDocument1->addResource(QTextDocument::ImageResource, QUrl("name"), QVariant(img));
        ui.textEdit->textCursor().insertImage("name");
    }
}
