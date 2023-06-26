#pragma once

#include <QDialog>
#include "ui_QtSetCompose.h"
#include "QtChildWidget.h"

class QtSetCompose : public QDialog
{
	Q_OBJECT

public:
	QtSetCompose(QWidget *parent = nullptr,QStringList list = QStringList());
	~QtSetCompose();

	void setComboxKeys(QStringList);
	void setCurrentValues(QVariantMap);


public slots:
	void clickButtonAdd(QString key = QString(),int v = 0 );
	void clickButtonDel();
	void clickOk();
signals:
	void sendKV(QMap<QString,int>);

private:
	int getChildWidgetCount();
private:
	QStringList gList;

private:
	Ui::QtSetComposeClass ui;
};
