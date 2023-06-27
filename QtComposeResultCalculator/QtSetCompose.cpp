#include "stdafx.h"
#include "QtSetCompose.h"

QtSetCompose::QtSetCompose(QWidget *parent, QStringList list)
	: QDialog(parent)
	, gList(list)
{
	ui.setupUi(this);

	//构造默认调用两遍?
	//clickButtonAdd();
	//clickButtonAdd();
	//gList = new QStringList("");
	QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(clickButtonAdd()));
	QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(clickButtonDel()));
	QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(clickOk()));
}

QtSetCompose::~QtSetCompose()
{}

void QtSetCompose::setComboxKeys(QStringList list)
{
	gList = list;
}

void QtSetCompose::setCurrentValues(QVariantMap vMap)
{
	int count = ui.verticalLayout->count();
	if (getChildWidgetCount() < 1)
	{
		QMap<QString,QVariant>::iterator it = vMap.begin();
		for (it; it != vMap.end(); it++)
		{
			clickButtonAdd(it.key(),it.value().toInt());
		}
	}
}

void QtSetCompose::setCurrentShow(QString str, QPixmap pixmap)
{

	//QBitmap bitM;
	//bitM.createHeuristicMask(false);
	//pixmap.setMask(bitM);
#if 1
	QImage image = pixmap.toImage();
	image = image.convertToFormat(QImage::Format_ARGB32);
	union myrgb
	{
		uint rgba;
		uchar rgba_bits[4];
	};
	myrgb* mybits = (myrgb*)image.bits();
	int len = image.width() * image.height();
	while (len-- > 0)
	{
		if (mybits->rgba_bits[0] == 0 && mybits->rgba_bits[1] == 0 && mybits->rgba_bits[2] == 0)
		{
			mybits->rgba_bits[3] = (mybits->rgba == 0xFFFFFFFF) ? 255 : 0;
		}
		mybits++;
	}
#endif
	//QBitmap alphaChannel = pixmap.createMaskFromColor(QColor(255, 255, 255, 255), Qt::MaskInColor);
	//image.setAlphaChannel(alphaChannel.toImage());
#if 0
	QPixmap temp(pixmap.size());
	temp.fill(Qt::transparent);

	QPainter p1(&temp);
	p1.setCompositionMode(QPainter::CompositionMode_Source);
	p1.drawPixmap(0, 0, pixmap);
	p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);

	//根据QColor中第四个参数设置透明度，0～255
	p1.fillRect(temp.rect(), QColor(0, 0, 0, 125));
	p1.end();

	pixmap = temp;
#endif

	//按照比例缩放
	//QPixmap TempPixmap = pixmap.scaled(ui.label->width(), ui.label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.label->setScaledContents(true);
	ui.label->setPixmap(QPixmap::fromImage(image).scaled(100,100));
	ui.label->setAlignment(Qt::AlignCenter);

	//QString mQss = "border-image:url(:/image/bg.jpg);color:rgb(128,128,128);";
	//ui.label->setStyleSheet(mQss);
	//ui.label->setText("This is style sheet");
	// tell the painter to draw on the QImage
	//QPainter* painter = new QPainter(&pixmap); // sorry i forgot the "&"
	//painter->setPen(Qt::blue);
	//painter->setFont(QFont("Arial", 30));
	//painter->drawText(pixmap.rect(), Qt::AlignCenter, str);
}


void QtSetCompose::clickButtonAdd(QString key, int v)
{
	QtChildWidget* childWiget = new QtChildWidget(this);
	childWiget->setComboxList(gList);
	childWiget->setCurrentKeyValue(key,v);
	
	int count = ui.verticalLayout->count();
	//qDebug() << "增加之前垂直布局的数量:" << count;
	if (count < 1)
	{
		count = 1;
	}
	ui.verticalLayout->insertWidget(count-1,childWiget);
}
void QtSetCompose::clickButtonDel()
{
	int count = ui.verticalLayout->count();
	if (count > 1)
	{
		QLayoutItem* child;
		if ((child = ui.verticalLayout->takeAt(count-2)) != nullptr) {
			delete child->widget(); 
			delete child;
		}
	}
}

void QtSetCompose::clickOk()
{
	int count = ui.verticalLayout->count();
	QMap<QString, int> smap;
	for (size_t i = 0; i < count; i++)
	{
		QLayoutItem* lItem = ui.verticalLayout->itemAt(i);
		QtChildWidget *widget = dynamic_cast<QtChildWidget*>(lItem->widget());
		if (widget)
		{
			QMap<QString, int> map = widget->getKeyValue();
			if (smap.contains(map.firstKey()))
			{
				int sum = smap.value(map.firstKey()) + map.value(map.firstKey());
				smap.remove(map.firstKey());
				smap.insert(map.firstKey(), sum);
			}
			else smap.insert(map);
		}
	}
	emit sendKV(smap);
}

int QtSetCompose::getChildWidgetCount()
{
	return ui.verticalLayout->count()-1;
}
