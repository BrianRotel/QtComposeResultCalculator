#include "stdafx.h"
#include "QtWatting.h"
#include <QMovie>

QtWatting::QtWatting(QWidget *parent)
	: QDialog(parent)
	, m_Move(NULL)
{
	ui.setupUi(this);
	//设置透明度
	this->setWindowOpacity(0.8);

	//取消对话框标题
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);//设置为对话框风格，并且去掉边框
	setWindowModality(Qt::WindowModal);//设置为模式对话框，同时在构造该对话框时要设置父窗口
	ui.lWatting->setStyleSheet("background-color: transparent;");

	m_Move = new QMovie(":/QtComposeResultCalculator/Resource/watting.gif");
	ui.lWatting->setMovie(m_Move);
	ui.lWatting->setScaledContents(true);
	bool isCon = QObject::connect(m_Move, &QMovie::stateChanged, this, &QtWatting::checkState);
	m_Move->start();
}

QtWatting::~QtWatting()
{
	m_Move->stop();
}
void QtWatting::checkState(QMovie::MovieState state)
{
	qDebug() << state;
}
