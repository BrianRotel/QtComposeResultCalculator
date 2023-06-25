#include "stdafx.h"
#include "QtSetCompose.h"

QtSetCompose::QtSetCompose(QWidget *parent, QStringList list)
	: QDialog(parent)
	, gList(list)
{
	ui.setupUi(this);

	//构造默认调用两遍?
	clickButtonAdd();
	clickButtonAdd();
	//gList = new QStringList("");
	QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(clickButtonAdd()));
	QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(clickButtonDel()));
	QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(clickOk()));
}

QtSetCompose::~QtSetCompose()
{}

void QtSetCompose::setKeys(QStringList list)
{
	gList = list;
}


void QtSetCompose::clickButtonAdd()
{
	QtChildWidget* childWiget = new QtChildWidget(this);
	childWiget->setComboxList(gList);
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