#include "stdafx.h"
#include "QtComposeResultCalculator.h"
#include "QtGetWindowInfo.h"
#include "QtGrabWindow.h"
#include <QtWidgets/QApplication>
#include <QScreen>

#include <iostream>
#include <string>
#include "DXGIDuplicator.h"

void SaveBmp(std::string filename, const uint8_t* data, int width, int height)
{
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == NULL)
    {
        return;
    }
    // 已写入字节数
    DWORD bytesWritten = 0;
    // 位图大小，颜色默认为32位即rgba
    int bmpSize = width * height * 4;

    // 文件头
    BITMAPFILEHEADER bmpHeader;
    // 文件总大小 = 文件头 + 位图信息头 + 位图数据
    bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpSize;
    // 固定
    bmpHeader.bfType = 0x4D42;
    // 数据偏移，即位图数据所在位置
    bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    // 保留为0
    bmpHeader.bfReserved1 = 0;
    // 保留为0
    bmpHeader.bfReserved2 = 0;
    // 写文件头
    WriteFile(hFile, (LPSTR)&bmpHeader, sizeof(bmpHeader), &bytesWritten, NULL);

    // 位图信息头
    BITMAPINFOHEADER bmiHeader;
    // 位图信息头大小
    bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    // 位图像素宽度
    bmiHeader.biWidth = width;
    // 位图像素高度，负高度即上下翻转
    bmiHeader.biHeight = -height;
    // 必须为1
    bmiHeader.biPlanes = 1;
    // 像素所占位数
    bmiHeader.biBitCount = 32;
    // 0表示不压缩
    bmiHeader.biCompression = 0;
    // 位图数据大小
    bmiHeader.biSizeImage = bmpSize;
    // 水平分辨率(像素/米)
    bmiHeader.biXPelsPerMeter = 0;
    // 垂直分辨率(像素/米)
    bmiHeader.biYPelsPerMeter = 0;
    // 使用的颜色，0为使用全部颜色
    bmiHeader.biClrUsed = 0;
    // 重要的颜色数，0为所有颜色都重要
    bmiHeader.biClrImportant = 0;

    // 写位图信息头
    WriteFile(hFile, (LPSTR)&bmiHeader, sizeof(bmiHeader), &bytesWritten, NULL);
    // 写位图数据
    WriteFile(hFile, data, bmpSize, &bytesWritten, NULL);
    CloseHandle(hFile);
}

void SaveDesktopImage(std::string filename, ID3D11Texture2D* texture2D, DXGIDuplicator* duplicator)
{
    D3D11_TEXTURE2D_DESC desc;
    texture2D->GetDesc(&desc);
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    duplicator->deviceContext_->Map(texture2D, 0, D3D11_MAP_READ, 0, &mappedResource);

    size_t imageSize = desc.Width * desc.Height * 4;
    uint8_t* rgba = (uint8_t*)malloc(imageSize);
    if (rgba == nullptr)
    {
        return;
    }
    memset(rgba, 0, imageSize);
    uint8_t* pData = (uint8_t*)mappedResource.pData;
    for (size_t i = 0; i < desc.Height; i++)
    {
        memcpy(rgba + i * desc.Width * 4, pData + i * mappedResource.RowPitch, desc.Width * 4);
    }
    SaveBmp(filename, rgba, desc.Width, desc.Height);
    free(rgba);
}
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    DXGIDuplicator* duplicator = new DXGIDuplicator;

    if (!duplicator->InitD3D11Device())
    {
        std::cout << "Init d3d11 device failed" << std::endl;
        return 1;
    }

    if (!duplicator->InitDuplication())
    {
        std::cout << "Init duplication failed" << std::endl;
        return 1;
    }

    ID3D11Texture2D* texture2D = nullptr;
    int num = 0;
    while (num < 10)
    {
        if (!duplicator->GetDesktopFrame(&texture2D))
        {
            std::cout << "Acquire frame failed" << std::endl;
            return 1;
        }
        if (texture2D == nullptr)
        {
            std::cout << "Acquire frame timeout" << std::endl;
            continue;
        }
        std::string filename = "desktop" + std::to_string(num) + ".bmp";
        SaveDesktopImage(filename, texture2D, duplicator);
        std::cout << filename << std::endl;
        texture2D->Release();
        texture2D = nullptr;
        num++;
        Sleep(500);
    }

    delete duplicator;
#if 0
	std::shared_ptr<QtGrabWindow> screen_capture = std::make_shared<QtGrabWindow>();
	if (!screen_capture.get()->InitD3D11Device()) {
		return  -1;
	}

	if (!screen_capture.get()->InitDuplication()) {
		return -1;
	}

	int counts = 0;
	/* 每隔1秒获取一次图像 */
	while (counts < 5) {
		QString fileName = QString("%1.png").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
		screen_capture.get()->GetDesktopFrame(fileName);

		counts++;
		Sleep(1000);
	}
#endif
    QtGrabWindow c;
    c.show();
    QtGetWindowInfo b;
    b.show();
    QtComposeResultCalculator w;
    w.show();
    return a.exec();
}
