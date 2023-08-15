#include "stdafx.h"
#include "QtGetWindowInfo.h"
#include "QtCrosshairLabelClick.h"
#include <QMetaType>
#include <tlhelp32.h>
//#define Get_ALL_Window
// 三种获取所有进程的方法调用在构造的 #if 0 代码块中
#ifdef Get_ALL_Window
#include <Windows.h>
#include <iostream>
#include <tchar.h>
struct ProcessInfo {

	QString processID;              //进程ID
	QString threadNum;              //该进程开启的线程数
	QString parentProcessID;        //父进程ID
	QString processPri;             //线程优先权
	QString processHandle;          //进程句柄
	QString processName;            //名称
};
QVector<ProcessInfo> m_vec;
void insertInfo(const QString& pID, const QString& thrNum,
	const QString& parePID, const QString& processPr,
	const QString& handle, const QString& processName)
{
	ProcessInfo info;
	info.processID = pID;
	info.threadNum = thrNum;
	info.parentProcessID = parePID;
	info.processPri = processPr;
	info.processHandle = handle;
	info.processName = processName;

	m_vec.append(info);
}
void getSnapshot()
{
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (!hProcess) {

		qDebug() << "CreateToolhelp32Snapshot failed!";
		return;
	}

	PROCESSENTRY32 info;
	info.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcess, &info)) {

		qDebug() << "Process32First null";
		return;
	}

	bool haveNext = true;
	while (haveNext) {

		haveNext = Process32Next(hProcess, &info);
		if (haveNext) {

			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, info.th32ProcessID);

			insertInfo(QString::number(info.th32ProcessID), QString::number(info.cntUsage),
				QString::number(info.th32ParentProcessID), QString::number(info.pcPriClassBase),
				QString::number((DWORD)hProcess), QString::fromWCharArray(info.szExeFile));
		}
	}
}
char m_Name[MAXBYTE];
char m_Title[MAXBYTE];
WORD m_nNum;
//系统返还给你的窗口句柄,API调用进来的参数
BOOL CALLBACK enumAllWindow(HWND Hwnd, LPARAM IParm)
{
	//每次Hwnd返回回来，都需要获取他的类名和标题
	GetClassNameA(Hwnd, m_Name, MAXBYTE);//获得指定窗口所属的类的类名
	GetWindowTextA(Hwnd, m_Title, MAXBYTE);//查找标题
	m_nNum++;
	QString strName = QString::fromLocal8Bit(m_Name);
	QString strTitle = QString::fromLocal8Bit(m_Title);
	qDebug() << "ID:" << m_nNum << "窗口句柄:" << Hwnd << "窗口类名:" << strName << "窗口标题:" << strTitle;
	//    return false;//枚举一次就不枚举了
	return true;//枚举到完毕
}

/* 回调函数，用于捕获进程 */
BOOL MyEnumProc(HWND hwnd, LPARAM param)
{
	LPWSTR lpString = (LPWSTR)malloc(sizeof(WCHAR) * MAX_PATH);
	if (IsWindow(hwnd) &&
		IsWindowEnabled(hwnd) &&
		IsWindowVisible(hwnd))
	{
		if (GetWindowTextW(hwnd, lpString, MAX_PATH) > 0) {
			qDebug() << QString::fromStdWString(lpString); // 打印出有标题的窗口
		}
	}
	free(lpString);
	return TRUE;
}
static BOOL CALLBACK enumchildWindowCallback(HWND hWnd, LPARAM lparam) {
	TCHAR buffer[256] = {};
	GetWindowText(hWnd, buffer, 256);
	int n = (int)hWnd;
	if (IsWindowVisible(hWnd))
	{
		RECT rc;
		GetWindowRect(hWnd, &rc);
		//_tprintf(TEXT("0x%x : %s  %d %d\n"), n, buffer, rc.right - rc.left, rc.bottom - rc.top);
		qDebug() << (TEXT("0x%x \n"), n) << QString::fromStdWString(buffer) << rc.right - rc.left << rc.bottom - rc.top; // 打印出有标题的窗口
	}
	return TRUE;
}

static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM m) {
	TCHAR buffer[256] = {};
	GetWindowText(hWnd, buffer, 256);
	int n = (int)hWnd;
	if (IsWindowVisible(hWnd))
	{
		RECT rc;
		GetWindowRect(hWnd, &rc);
		//_tprintf(TEXT("0x%x : %s  %d %d\n"), n, buffer, rc.right - rc.left, rc.bottom - rc.top);
		//qDebug() << (TEXT("0x%x : %s  %d %d\n"), n, buffer, rc.right - rc.left, rc.bottom - rc.top); // 打印出有标题的窗口
		qDebug() << (TEXT("0x%x \n"), n) << QString::fromStdWString(buffer) << rc.right - rc.left << rc.bottom - rc.top; // 打印出有标题的窗口
		EnumChildWindows(hWnd, enumchildWindowCallback, m);
	}
	return TRUE;
}
#endif
QtGetWindowInfo::QtGetWindowInfo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//多线程适用
	qRegisterMetaType<WindowInfo>("WindowInfo");

#ifdef Get_ALL_Window //可用
	//LPCWSTR className = TEXT("UnityWndClass");//UnrealWindow UnityWndClass
	//LPCWSTR winName = TEXT("IdleSpiral");//剑灵 IdleSpiral
	//HWND hwnd = FindWindow(className, winName);
	//if (IsWindowVisible(hwnd))
	//{
	//	EnumChildWindows(hwnd, enumchildWindowCallback, NULL);
	//}
#if 1
	//[0]获取所有进程及其子窗口
	///_tprintf(TEXT("Enmumerating windows..."));
	//EnumWindows(enumWindowCallback, NULL);
	//[0.1]根据父窗口获取子窗口
	//HWND hwnd = (HWND)0x000D0DEE; //父窗口的句柄
	//EnumChildWindows(hwnd, enumchildWindowCallback, NULL);
	//[0.1]根据父窗口获取子窗口
	//[0]获取所有进程及其子窗口

	//[1]获取所有进程
	getSnapshot();
	for (size_t i = 0; i < m_vec.length(); i++)
	{

		//if (m_vec.at(i).processName.contains("IdleSpiral"))//GameOverlayUI.exe
		{
			qDebug() <<
				"进程ID" << m_vec.at(i).processID << " " <<
				"该进程开启的线程数" << m_vec.at(i).threadNum << " " <<
				"父进程ID" << m_vec.at(i).parentProcessID << " " <<
				"线程优先权" << m_vec.at(i).processPri << " " <<
				"进程句柄" << m_vec.at(i).processHandle << " " <<
				"名称"<< m_vec.at(i).processName;
		}
	}
#endif
#if 0

	//[1]获取所有进程
	//[2]获取所有窗口
	m_nNum = 0;
	EnumWindows(enumAllWindow, (LPARAM)"");
	//[2]
	//[3]获取所有窗口(只是含有标题)
	//EnumWindows(MyEnumProc, 0);
	//[3]
#endif // 0
#endif
	myLabel = new QtCrosshairLabelClick();
	
	//ui.horizontalLayout->insertWidget(1, myLabel);
	ui.horizontalLayout->addWidget(myLabel);
	m_timer = new QTimer();
	bool isConnect = QObject::connect(myLabel, SIGNAL(windowInfo(const WindowInfo&)), this, SLOT(slotShowValue(const WindowInfo&)));
	if (!isConnect)
	{
		qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "QtGetWindowInfo Construct Function connect QtCrosshairLabelClick-->QtGetWindowInfo Failed";
	}
	isConnect = connect(m_timer, &QTimer::timeout, this, &QtGetWindowInfo::slotTimeFindWindowName);
	//isConnect = QObject::connect(ui.pushButton, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_getPointWindowName_clicked(bool)));
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

void QtGetWindowInfo::slotShowValue(const WindowInfo&info)
{
	ui.lineEdit_windowHwnd->setText(info.windowSHwnd);
	ui.lineEdit_windowTitle->setText(info.windowTitle);
	ui.lineEdit_windowClass->setText(info.windowClass);
	ui.lineEdit_windowStyle->setText(info.windowStyle);
	ui.lineEdit_windowRect->setText(info.windowRect);
}
