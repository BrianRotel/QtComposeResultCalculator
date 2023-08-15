#pragma once

#include <QMainWindow>
#include "ui_QtGetScreenColor.h"

class QtGetScreenColor : public QMainWindow
{
	Q_OBJECT

public:
	QtGetScreenColor(QWidget *parent = nullptr);
	~QtGetScreenColor();

	void showColorValue();

private:
	Ui::QtGetScreenColorClass ui;
	QTimer *timer;
};
