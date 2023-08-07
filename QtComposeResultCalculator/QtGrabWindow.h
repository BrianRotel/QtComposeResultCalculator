#pragma once

#include <QMainWindow>
#include "ui_QtGrabWindow.h"

#include <QGuiApplication>
#include <QScreen>
//#include <QDesktopWidget>
#include <Qpainter>

#include "windows.h"
#include "winuser.h"
#include <QWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_2.h>

QT_BEGIN_NAMESPACE
namespace Ui { class QtGrabWindow; }
QT_END_NAMESPACE
typedef BOOL(__stdcall* PtrPrintWindow)(HWND, HDC, UINT);

#define RESET_OBJECT
#define GDI 1

class QtGrabWindow : public QMainWindow
{
	Q_OBJECT

public:
	QtGrabWindow(QWidget *parent = nullptr);
	~QtGrabWindow();

	void paintEvent(QPaintEvent * e) override;
	QImage myGrabWindow(WId winId, bool needMouse);
	PtrPrintWindow printWindow();
#ifdef RESET_OBJECT
	bool Init();
	bool QueryFrame(QRect& rect, void* pImgData, INT& nImgSize);
	bool copyImageByRect(char* src, QSize size, char* dst, int imgSize, QRect rect);
#endif // 0

	bool InitD3D11Device();//b
	bool InitDuplication();//b
	QImage GetDesktopFrame(QString fileName = "");//b
	QImage CopyDesktopImage(ID3D11Texture2D* texture);//b
	void timerEvent(QTimerEvent * e);

private:
	Ui::QtGrabWindowClass ui;
	QScreen * m_pScreen = nullptr;
	QWindow * calcWin;
	QWidget * calcWidget;
	QWidget * owidget;
private:

#ifdef RESET_OBJECT
	ID3D11Device* m_hDevice = nullptr;//a
	ID3D11DeviceContext* m_hContext = nullptr;//a
	DXGI_OUTPUT_DESC m_dxgiOutDesc;//a
	IDXGIOutputDuplication* m_hDeskDupl;//a
	bool m_bInit;
#endif // 0
	ID3D11Device* m_pDevice = nullptr;//b
	IDXGIOutputDuplication* m_pDuplication;//b
	ID3D11DeviceContext* m_pDeviceContext;//b
	int m_screenWidth;
	int m_screenHeight;
};
