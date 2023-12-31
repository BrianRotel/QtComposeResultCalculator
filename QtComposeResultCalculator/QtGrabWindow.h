﻿#pragma once

#include "ui_QtGrabWindow.h"
#include <QGuiApplication>
#include <QWindow>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <Qpainter>
#include <QScreen>
#include <QDebug>
//#include <QDesktopWidget>

#include "windows.h"
#include "winuser.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_2.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class QtGrabWindow; }
QT_END_NAMESPACE
typedef BOOL(__stdcall* PtrPrintWindow)(HWND, HDC, UINT);

#define RESET_OBJECT
#define GDI 1
//#define DXGI_TEST 1
//#define DXGI 1

class QtGrabWindow : public QMainWindow
{
	Q_OBJECT

public:
	QtGrabWindow(QWidget *parent = nullptr);
	~QtGrabWindow();
	void paintEvent(QPaintEvent * e) override;
#ifdef GDI
	QImage myGrabWindow(WId winId, bool needMouse);
	PtrPrintWindow printWindow();
#endif // GDI
#ifdef DXGI_TEST
	bool Init();
	bool QueryFrame(QRect& rect, void* pImgData, INT& nImgSize);
	bool copyImageByRect(char* src, QSize size, char* dst, int imgSize, QRect rect);
#endif // DXGI_TEST

#ifdef DXGI
	bool InitD3D11Device();//DXGI
	bool InitDuplication();//DXGI
	QImage GetDesktopFrame(QString fileName = "");//DXGI
	QImage CopyDesktopImage(ID3D11Texture2D* texture);//DXGI
#endif // DXGI
	void timerEvent(QTimerEvent * e);
private slots:
	void slotTimer();
private:
	QImage findPicture(QImage src, QImage child);
	void getPosition();
	void mouseMoveEvent(QMouseEvent* event);
	void clickOther();
private:
	Ui::QtGrabWindowClass ui;
	QScreen* m_pScreen = nullptr;
	QWindow* calcWin;
	QWidget* calcWidget;
	QWidget* owidget;
	QTimer* timer;
	LPCWSTR className;
	LPCWSTR winName;
	HWND hwnd;
	QPoint clickP;
	int bx;
	int by;
private:

#ifdef DXGI_TEST
	ID3D11Device* m_hDevice = nullptr;//DXGI_TEST
	ID3D11DeviceContext* m_hContext = nullptr;//DXGI_TEST
	DXGI_OUTPUT_DESC m_dxgiOutDesc;//DXGI_TEST
	IDXGIOutputDuplication* m_hDeskDupl;//DXGI_TEST
	bool m_bInit;
#endif // DXGI_TEST
#ifdef DXGI
	ID3D11Device* m_pDevice = nullptr;//DXGI
	IDXGIOutputDuplication* m_pDuplication;//DXGI
	ID3D11DeviceContext* m_pDeviceContext;//DXGI
	int m_screenWidth;
	int m_screenHeight;
#endif // DXGI
};
