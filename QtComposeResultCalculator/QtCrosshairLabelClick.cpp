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
	//获取窗口句柄
	HWND hWnd = WindowFromPoint(POINT{ event->globalPos().x(), event->globalPos().y() });
	if (info.windowHwnd == hWnd)//重复性校验,优化卡顿
	{
		return;
	}
	if (hWnd != 0) {

		//获取窗口标题
		TCHAR szTitle[MAX_PATH];
		GetWindowText(hWnd, szTitle, MAX_PATH);
		QString windowsTitle = QString::fromWCharArray(szTitle);

		//获取窗口类名
		TCHAR szClass[MAX_PATH];
		GetClassName(hWnd, szClass, MAX_PATH);
		QString windowsClass = QString::fromWCharArray(szClass);
		//获取窗口样式
		LONG style = GetWindowLong(hWnd, GWL_STYLE);
		QString windowsStyle = QString("%1").arg(style, QT_POINTER_SIZE, 16, QChar('0'));
		//获取窗口矩形
		RECT rect;
		GetWindowRect(hWnd, &rect);
		QRect window_rect(rect.left, rect.top, rect.right, rect.bottom);
		QString windowsRect = QString("(%1,%2)-(%3,%4) %5×%6")\
			.arg(window_rect.topLeft().x())\
			.arg(window_rect.topLeft().y())\
			.arg(window_rect.bottomRight().x())\
			.arg(window_rect.bottomRight().y())\
			.arg(window_rect.width())\
			.arg(window_rect.height());
		QString windowsHwnd = QString("%1").arg((quintptr)hWnd, QT_POINTER_SIZE, 16, QLatin1Char('0'));
		info.windowHwnd = hWnd;
		info.windowSHwnd = windowsHwnd;
		info.windowTitle = windowsTitle;
		info.windowClass = windowsClass;
		info.windowStyle = windowsStyle;
		info.windowRect = windowsRect;
	}
#if 0
	HWND hwnd = ::WindowFromPoint(POINT{ event->globalPos().x(), event->globalPos().y() });
	char title[256] = { 0 };
	::GetWindowTextA(hwnd, title, sizeof(title));
	QString titleStr = QString::fromLocal8Bit(title);

	char className[256] = { 0 };
	::GetClassNameA(hwnd, className, sizeof(className));
	QString classNameStr = QString::fromLocal8Bit(className);
#endif
	emit windowInfo(info);
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


