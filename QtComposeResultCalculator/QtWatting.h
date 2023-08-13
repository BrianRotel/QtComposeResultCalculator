#pragma once

#include <QDialog>
#include "ui_QtWatting.h"

class QtWatting : public QDialog
{
	Q_OBJECT

public:
	QtWatting(QWidget *parent = nullptr);
	~QtWatting();

	void checkState(QMovie::MovieState state);

private:
	Ui::QtWattingClass ui;
	QMovie* m_Move;
};
