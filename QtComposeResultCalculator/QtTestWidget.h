#pragma once

#include <QMainWindow>
#include "ui_QtTestWidget.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

class QtTestWidget : public QMainWindow
{
	Q_OBJECT

public:
	QtTestWidget(QWidget *parent = nullptr);
	~QtTestWidget();

	void Transfer();
protected:

	void mouseMoveEvent(QMouseEvent* event) override;
private:
	Ui::QtTestWidgetClass ui;
};
