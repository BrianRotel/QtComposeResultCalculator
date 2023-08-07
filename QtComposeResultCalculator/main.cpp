#include "stdafx.h"
#include "QtComposeResultCalculator.h"
#include "QtGetWindowInfo.h"

#include <QtWidgets/QApplication>
#include <QScreen>
#include <windows.h>
#include <tlhelp32.h>
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
BOOL CALLBACK enumAllWindow(HWND Hwnd, LPARAM IParm)//系统返还给你的窗口句柄,API调用进来的参数
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

int main(int argc, char *argv[])
{
    //获取所有进程
    getSnapshot();
    for (size_t i = 0; i < m_vec.length(); i++)
    {

        if (m_vec.at(i).processName.contains("IdleSpiral"))
        {
            qDebug() <<
                m_vec.at(i).processID << " " <<
                m_vec.at(i).threadNum << " " <<
                m_vec.at(i).parentProcessID << " " <<
                m_vec.at(i).processPri << " " <<
                m_vec.at(i).processHandle << " " <<
                m_vec.at(i).processName;
        }
    }
    //获取所有窗口
    m_nNum = 0;
    EnumWindows(enumAllWindow, (LPARAM)"");

    //获取所有窗口(只是含有标题)
    EnumWindows(MyEnumProc, 0);

    //QScreen* screen = QGuiApplication::primaryScreen();
    //if (screen) {
    //    QPixmap originalPixmap = screen->grabWindow(0);
    //}

    //HWND AppWnd;
    //QString ClassName("UnityWndClass");
    //QString AppName("IdleSpiral");

    //LPCWSTR App = reinterpret_cast <LPCWSTR>(ClassName.data());
    //LPCWSTR AppCaption = reinterpret_cast <LPCWSTR>(AppName.data());
    //AppWnd = ::FindWindowW(App, AppCaption);
    //if (!AppWnd) {
    //    QMessageBox::information(nullptr, ("Warning"), ("can't get IJPAS Window Handle!"));
    //};

    //HWND appwnd;
    //appwnd = (HWND)w.winId();
    //QString ClassName("Qt5QWindowIcon");
    //QString AppName = QStringLiteral("GG");

    //LPCWSTR app = reinterpret_cast<LPCWSTR>(ClassName.data());
    //LPCWSTR appCaption = reinterpret_cast<LPCWSTR>(AppName.data());

    //appwnd = ::FindWindow(app, appCaption);
    //appwnd = ::FindWindow(app, appCaption);

    //QDesktopWidget::screenCount()
    //WId desktop = GetDesktopWindow();
    //WId desktop2 = QApplication::desktop()->winId();
    //if (desktop != desktop2)
    //{
    //    qDebug("sunt fdiferite WId");
    //}
    //QPixmap entireDesktop = QPixmap::grabWindow(desktop);




    QApplication a(argc, argv);
    QtGetWindowInfo b;
    b.show();
    QtComposeResultCalculator w;
    w.show();
    return a.exec();
}
