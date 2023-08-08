#pragma once

#include <QWidget>
#include "ui_QtMyClipboard.h"

class QtMyClipboard : public QWidget
{
	Q_OBJECT

public:
	QtMyClipboard(QWidget *parent = nullptr);
	~QtMyClipboard();
	void Init();
#ifdef UIWIDGET
	virtual void resizeEvent(QResizeEvent* event);
#endif
private slots:
#ifdef UIWIDGET
	void onPushButton1(bool b);
#endif
	void onPushButton2(bool b);
private:
	Ui::QtMyClipboardClass ui;
};
