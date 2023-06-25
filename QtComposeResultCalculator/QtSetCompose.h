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

	void setKeys(QStringList);


public slots:
	void clickButtonAdd();
	void clickButtonDel();
	void clickOk();
signals:
	void sendKV(QMap<QString,int>);
private:
	QStringList gList;
private:
	Ui::QtSetComposeClass ui;
};
