#include "stdafx.h"
#include "QtGetWindowInfo.h"

QtGetWindowInfo::QtGetWindowInfo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_timer = new QTimer();
	connect(m_timer, &QTimer::timeout, this, &QtGetWindowInfo::slotTimeFindWindowName);
	QObject::connect(ui.pushButton, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_getPointWindowName_clicked(bool)));
}

QtGetWindowInfo::~QtGetWindowInfo()
{}

void QtGetWindowInfo::on_pushButton_getPointWindowName_clicked(bool checked)
{
	if (checked) {
		this->setCursor(Qt::PointingHandCursor);
		if (m_timer->isActive())
			m_timer->stop();
		m_timer->start(500);
	}
	else {
		this->setCursor(Qt::ArrowCursor);
		m_timer->stop();
	}
}

void QtGetWindowInfo::slotTimeFindWindowName()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	//获取窗口句柄
	HWND hWnd = WindowFromPoint(ptCursor);
	if (hWnd != 0) {
		//获取窗口标题
		TCHAR szTitle[MAX_PATH];
		GetWindowText(hWnd, szTitle, MAX_PATH);
		//获取窗口类名
		TCHAR szClass[MAX_PATH];
		GetClassName(hWnd, szClass, MAX_PATH);
		//获取窗口样式
		LONG style = GetWindowLong(hWnd, GWL_STYLE);
		//获取窗口矩形
		RECT rect;
		GetWindowRect(hWnd, &rect);
		QRect window_rect(rect.left, rect.top, rect.right, rect.bottom);

		QString windowsHwnd = QString("%1").arg((quintptr)hWnd, QT_POINTER_SIZE, 16, QLatin1Char('0'));
		QString windowsTitle = QString::fromWCharArray(szTitle);
		QString windowsClass = QString::fromWCharArray(szClass);
		QString windowsStyle = QString("%1").arg(style, QT_POINTER_SIZE, 16, QChar('0'));
		QString windowsRect = QString("(%1,%2)-(%3,%4) %5×%6")\
			.arg(window_rect.topLeft().x())\
			.arg(window_rect.topLeft().y())\
			.arg(window_rect.bottomRight().x())\
			.arg(window_rect.bottomRight().y())\
			.arg(window_rect.width())\
			.arg(window_rect.height());

		ui.lineEdit_windowHwnd->setText(windowsHwnd);
		ui.lineEdit_windowTitle->setText(windowsTitle);
		ui.lineEdit_windowClass->setText(windowsClass);
		ui.lineEdit_windowStyle->setText(windowsStyle);
		ui.lineEdit_windowRect->setText(windowsRect);
	}
}