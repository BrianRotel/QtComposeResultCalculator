#include "stdafx.h"
#include "DXGIDuplicator.h"

// 构造函数，里面什么也不做
DXGIDuplicator::DXGIDuplicator()
{
}

// 析构函数，释放相关对象
DXGIDuplicator::~DXGIDuplicator()
{
    if (duplication_)
    {
        duplication_->Release();
    }
    if (device_)
    {
        device_->Release();
    }
    if (deviceContext_)
    {
        deviceContext_->Release();
    }
}
bool DXGIDuplicator::InitD3D11Device()
{
    D3D_DRIVER_TYPE DriverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT NumDriverTypes = ARRAYSIZE(DriverTypes);

    D3D_FEATURE_LEVEL FeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_1
    };
    UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
    D3D_FEATURE_LEVEL FeatureLevel;

    for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; ++DriverTypeIndex)
    {
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            DriverTypes[DriverTypeIndex],
            nullptr, 0,
            FeatureLevels,
            NumFeatureLevels,
            D3D11_SDK_VERSION,
            &device_,
            &FeatureLevel,
            &deviceContext_);
        if (SUCCEEDED(hr))
        {
            break;
        }
    }

    if (device_ == nullptr || deviceContext_ == nullptr)
    {
        return false;
    }

    return true;
}
bool DXGIDuplicator::InitDuplication()
{
    HRESULT hr = S_OK;

    IDXGIDevice* dxgiDevice = nullptr;
    hr = device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
    if (FAILED(hr))
    {
        return false;
    }

    IDXGIAdapter* dxgiAdapter = nullptr;
    hr = dxgiDevice->GetAdapter(&dxgiAdapter);
    dxgiDevice->Release();
    if (FAILED(hr))
    {
        return false;
    }

    UINT output = 0;
    IDXGIOutput* dxgiOutput = nullptr;
    while (true)
    {
        hr = dxgiAdapter->EnumOutputs(output++, &dxgiOutput);
        if (hr == DXGI_ERROR_NOT_FOUND)
        {
            return false;
        }
        else
        {
            DXGI_OUTPUT_DESC desc;
            dxgiOutput->GetDesc(&desc);
            int width = desc.DesktopCoordinates.right - desc.DesktopCoordinates.left;
            int height = desc.DesktopCoordinates.bottom - desc.DesktopCoordinates.top;
            break;
        }
    }
    dxgiAdapter->Release();

    IDXGIOutput1* dxgiOutput1 = nullptr;
    hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&dxgiOutput1));
    dxgiOutput->Release();
    if (FAILED(hr))
    {
        return false;
    }

    hr = dxgiOutput1->DuplicateOutput(device_, &duplication_);
    dxgiOutput1->Release();
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}
bool DXGIDuplicator::GetDesktopFrame(ID3D11Texture2D** texture)
{
    HRESULT hr = S_OK;
    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    IDXGIResource* resource = nullptr;
    ID3D11Texture2D* acquireFrame = nullptr;

    hr = duplication_->AcquireNextFrame(0, &frameInfo, &resource);
    if (FAILED(hr))
    {
        if (hr == DXGI_ERROR_WAIT_TIMEOUT)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&acquireFrame));
    resource->Release();
    if (FAILED(hr))
    {
        return false;
    }

    D3D11_TEXTURE2D_DESC desc;
    acquireFrame->GetDesc(&desc);
    desc.Usage = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    device_->CreateTexture2D(&desc, NULL, texture);
    if (texture && *texture)
    {
        deviceContext_->CopyResource(*texture, acquireFrame);
    }
    acquireFrame->Release();

    hr = duplication_->ReleaseFrame();
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}
//将下面两个函数移到main中可以保存为bitmap图片格式
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
//将下面的代码设置到主函数,调用上面的两个函数进行bitmap图片的保存
#if 0
int main(int argc, char* argv[])
{
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
}
#endif