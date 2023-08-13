#pragma once
#include <QWidget>
#include "ui_QtTestWidget.h"
#include "QtMyGraphicsView.h"
#include "QtMyClipboard.h"
#include "QtWatting.h"

class QtTestWidget : public QWidget
{
	Q_OBJECT

public:
	QtTestWidget(QWidget *parent = nullptr);
	~QtTestWidget();

	void Transfer();
	void doSomeThing();

	void doSomeThing2();

protected:

#ifdef SHOW_BACKGROUND
	void paintEvent(QPaintEvent* event) override;
#endif

private slots:
	void onPushButton(bool b);
	void onPushButton2(bool b);
	void onReceiveResult(QImage img);
private:
	Ui::QtTestWidgetClass ui;
private:
	QPixmap Qt_Image;
	QtMyGraphicsView* myView;
	QtMyClipboard myClip;
	QtWatting* pQwait;
};
