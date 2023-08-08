#include "stdafx.h"
#include "QtCrosshairLabelClick.h"

//弹出准星的可点击Label
QtCrosshairLabelClick::QtCrosshairLabelClick(QWidget* parent) : QLabel(parent) 
{
	QPixmap pixmap(":/QtComposeResultCalculator/Resource/Crosshair.png"); // 替换为你的图片路径
	pixmap = pixmap.scaled(QSize(30, 30), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	//setFixedSize(QSize(30,30));
	//setScaledContents(true);
	//pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	setPixmap(pixmap);

	m_CrosshairLabelMove = new QtCrosshairLabelMove();

	QObject::connect(this, &QtCrosshairLabelClick::mousePress, [&]() {
		m_CrosshairLabelMove->show();
		});

	QObject::connect(this, &QtCrosshairLabelClick::mouseMove, m_CrosshairLabelMove, &QtCrosshairLabelMove::updatePosition);
	QObject::connect(this, &QtCrosshairLabelClick::mouseRelease, [&]() {
		m_CrosshairLabelMove->hide();
		});
}
QtCrosshairLabelClick::~QtCrosshairLabelClick()
{

}
void QtCrosshairLabelClick::setCurrentPos(const QPoint& pos) {
	m_CrosshairLabelMove->setCurrentPos(pos);
}

void QtCrosshairLabelClick::mousePressEvent(QMouseEvent* event) {
	QLabel::mousePressEvent(event);
	if (event->button() == Qt::LeftButton) {
		emit mousePress(event->globalPos());
		QApplication::setOverrideCursor(QCursor(Qt::BlankCursor)); // 隐藏鼠标
	}
}

void QtCrosshairLabelClick::mouseMoveEvent(QMouseEvent* event) {
	QLabel::mouseMoveEvent(event);
	emit mouseMove(event->globalPos());
	HWND hwnd = ::WindowFromPoint(POINT{ event->globalPos().x(), event->globalPos().y() });
	char title[256] = { 0 };
	::GetWindowTextA(hwnd, title, sizeof(title));
	QString titleStr = QString::fromLocal8Bit(title);

	char className[256] = { 0 };
	::GetClassNameA(hwnd, className, sizeof(className));
	QString classNameStr = QString::fromLocal8Bit(className);
	emit windowInfo(titleStr, classNameStr, hwnd);
}

void QtCrosshairLabelClick::mouseReleaseEvent(QMouseEvent* event) {
	QLabel::mouseReleaseEvent(event);
	emit mouseRelease();

	QApplication::restoreOverrideCursor(); // 恢复鼠标
	this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void QtCrosshairLabelClick::hideEvent(QHideEvent* event) {
	QWidget::hideEvent(event);
	emit mouseRelease();

	QApplication::restoreOverrideCursor(); // 恢复鼠标
	this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
}


