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

class QtGrabWindow : public QMainWindow
{
	Q_OBJECT

public:
	QtGrabWindow(QWidget *parent = nullptr);
	~QtGrabWindow();

	void paintEvent(QPaintEvent * e) override;
	QImage myGrabWindow(WId winId, bool needMouse);
	PtrPrintWindow printWindow();
#if RESET_OBJECT
	bool Init();
	bool QueryFrame(QRect& rect, void* pImgData, INT& nImgSize);
#endif // 0


	bool InitD3D11Device();//b
	bool InitDuplication();//b
	bool GetDesktopFrame(QString fileName);//b
	QImage CopyDesktopImage(ID3D11Texture2D* texture);//b
	void drawOnce();
	void timerEvent(QTimerEvent * e);

private:
	Ui::QtGrabWindowClass ui;
	QScreen * m_pScreen = nullptr;
	QWindow * calcWin;
	QWidget * calcWidget;
	QWidget * owidget;
private:
	ID3D11Device* m_hDevice = nullptr;//a
	ID3D11DeviceContext* m_hContext = nullptr;//a
	DXGI_OUTPUT_DESC m_dxgiOutDesc;//a
	IDXGIOutputDuplication* m_hDeskDupl;//a
	ID3D11Device* m_pDevice = nullptr;//b
	IDXGIOutputDuplication* m_pDuplication;//b
	ID3D11DeviceContext* m_pDeviceContext;//b
	int m_screenWidth;
	int m_screenHeight;
	bool m_bInit;
};
