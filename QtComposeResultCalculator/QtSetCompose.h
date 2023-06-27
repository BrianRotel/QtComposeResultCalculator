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

	//设置第一个下拉框应该显示的值,除被点击者本身
	void setComboxKeys(QStringList);
	//设置当前点击的值的现状
	void setCurrentValues(QVariantMap);
	//当前设置的值
	void setCurrentShow(QString,QPixmap);


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
