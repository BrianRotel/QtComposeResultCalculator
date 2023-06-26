#include "stdafx.h"
#include "QtChildWidget.h"

QtChildWidget::QtChildWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//ui.comboBox->setCurrentText("");
	for (size_t i = 0; i < 100; i++)
	{
		gList.append(QString::number(i+1));
	}
	setComboxList2(gList);
}

QtChildWidget::~QtChildWidget()
{}

void QtChildWidget::setComboxList(QStringList list)
{
	ui.comboBox->addItems(list);
}

void QtChildWidget::setComboxList2(QStringList list)
{
	ui.comboBox_2->addItems(list);
}

QMap<QString, int> QtChildWidget::getKeyValue()
{
	QMap<QString, int> map;
	QString str = ui.comboBox->currentText();
	QString str2 = ui.comboBox_2->currentText();
	bool ok;
	int c = str2.toInt(&ok, 10);
	if (ok)
	{
		map.insert(str, c);
		return map;
	}
	return map;

}

void QtChildWidget::setCurrentKeyValue(QString key, int v)
{
	ui.comboBox->setCurrentText(key);
	ui.comboBox_2->setCurrentText(QString::number(v));
}
