#pragma once

#include <QWidget>
#include "ui_QtGetWindowInfo.h"
#include <QDebug>
#include <windows.h>
#include <QTimer>

class QtCrosshairLabelClick;
class QtGetWindowInfo : public QWidget
{
	Q_OBJECT

public:
	QtGetWindowInfo(QWidget *parent = nullptr);
	~QtGetWindowInfo();

private slots:
	void on_pushButton_getPointWindowName_clicked(bool checked);
	void slotTimeFindWindowName();

private:
	Ui::QtGetWindowInfoClass ui;
	QtCrosshairLabelClick* myLabel;
	QTimer* m_timer;
};
