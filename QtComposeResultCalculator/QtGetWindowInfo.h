#pragma once

#include <QWidget>
#include "ui_QtGetWindowInfo.h"
#include <QDebug>
#include <windows.h>
#include <QTimer>

class QtCrosshairLabelClick;
struct WindowInfo;
class QtGetWindowInfo : public QWidget
{
	Q_OBJECT

public:
	QtGetWindowInfo(QWidget *parent = nullptr);
	~QtGetWindowInfo();

private slots:
	void on_pushButton_getPointWindowName_clicked(bool checked);
	void slotTimeFindWindowName();
	void slotShowValue(const WindowInfo& info);

private:
	Ui::QtGetWindowInfoClass ui;
	QtCrosshairLabelClick* myLabel;
	QTimer* m_timer;
};
