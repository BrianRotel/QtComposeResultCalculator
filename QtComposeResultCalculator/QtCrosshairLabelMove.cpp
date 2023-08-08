#include "stdafx.h"
#include "QtCrosshairLabelMove.h"

//可移动准星Label

QtCrosshairLabelMove::QtCrosshairLabelMove(QWidget *parent)
	: QLabel(parent)
{
	this->setAttribute(Qt::WA_TransparentForMouseEvents);

	QPixmap pixmap(":/QtComposeResultCalculator/Resource/Crosshair.png"); // 替换为你的图片路径
	pixmap = pixmap.scaled(QSize(30, 30), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	this->setPixmap(pixmap);
	this->setMask(pixmap.mask()); // 使图像的透明部分被视为透明
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); // 使窗口无边框并始终保持在顶部
	this->setAttribute(Qt::WA_TranslucentBackground); // 设置背景为透明


	m_Timer = new QTimer(this);
	connect(m_Timer, &QTimer::timeout, this, &QtCrosshairLabelMove::checkFocus);
	m_Timer->start(100); // 检查频率，可以根据实际需要进行调整
}

QtCrosshairLabelMove::~QtCrosshairLabelMove()
{

}

void QtCrosshairLabelMove::setCurrentPos(const QPoint& pos)
{
	m_QPoint = pos;
}

void QtCrosshairLabelMove::showEvent(QShowEvent* event)
{

	int halfWidth = width() / 2;
	int halfHeight = height() / 2;
	move(m_QPoint.x() - halfWidth, m_QPoint.y() - halfHeight);
	QLabel::showEvent(event);
}


void QtCrosshairLabelMove::updatePosition(const QPoint& newPosition) {
	int halfWidth = width() / 2;
	int halfHeight = height() / 2;
	move(newPosition.x() - halfWidth, newPosition.y() - halfHeight);
}
void QtCrosshairLabelMove::checkFocus() {
	//这段代码首先检查当前窗口是否是活动窗口，并且是否可见。如果窗口不是活动的，或者不可见，那么就恢复鼠标，取消鼠标事件的透明处理，并隐藏准星。
	if (QApplication::activeWindow() != this || !this->isVisible()) {
		if (QApplication::overrideCursor() != nullptr) {
			QApplication::restoreOverrideCursor(); // 恢复鼠标
			this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
			if (this->isVisible()) {
				this->hide();
			}
		}
	}
}


