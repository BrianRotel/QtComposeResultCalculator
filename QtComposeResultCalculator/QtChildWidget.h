#pragma once

#include <QWidget>
#include "ui_QtChildWidget.h"

class QtChildWidget : public QWidget
{
	Q_OBJECT

public:
	QtChildWidget(QWidget *parent = nullptr);
	~QtChildWidget();
	void setComboxList(QStringList list);
	void setComboxList2(QStringList list);

	QMap<QString, int> getKeyValue();
	void setCurrentKeyValue(QString key, int v);
private:
	Ui::QtChildWidgetClass ui;
	QStringList gList;
};
