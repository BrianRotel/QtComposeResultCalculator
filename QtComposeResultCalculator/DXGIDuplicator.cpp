#include "stdafx.h"
#include "DXGIDuplicator.h"

// ���캯��������ʲôҲ����
DXGIDuplicator::DXGIDuplicator()
{
}

// �����������ͷ���ض���
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
//���������������Ƶ�main�п��Ա���ΪbitmapͼƬ��ʽ
void SaveBmp(std::string filename, const uint8_t* data, int width, int height)
{
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == NULL)
    {
        return;
    }
    // ��д���ֽ���
    DWORD bytesWritten = 0;
    // λͼ��С����ɫĬ��Ϊ32λ��rgba
    int bmpSize = width * height * 4;

    // �ļ�ͷ
    BITMAPFILEHEADER bmpHeader;
    // �ļ��ܴ�С = �ļ�ͷ + λͼ��Ϣͷ + λͼ����
    bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpSize;
    // �̶�
    bmpHeader.bfType = 0x4D42;
    // ����ƫ�ƣ���λͼ��������λ��
    bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    // ����Ϊ0
    bmpHeader.bfReserved1 = 0;
    // ����Ϊ0
    bmpHeader.bfReserved2 = 0;
    // д�ļ�ͷ
    WriteFile(hFile, (LPSTR)&bmpHeader, sizeof(bmpHeader), &bytesWritten, NULL);

    // λͼ��Ϣͷ
    BITMAPINFOHEADER bmiHeader;
    // λͼ��Ϣͷ��С
    bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    // λͼ���ؿ��
    bmiHeader.biWidth = width;
    // λͼ���ظ߶ȣ����߶ȼ����·�ת
    bmiHeader.biHeight = -height;
    // ����Ϊ1
    bmiHeader.biPlanes = 1;
    // ������ռλ��
    bmiHeader.biBitCount = 32;
    // 0��ʾ��ѹ��
    bmiHeader.biCompression = 0;
    // λͼ���ݴ�С
    bmiHeader.biSizeImage = bmpSize;
    // ˮƽ�ֱ���(����/��)
    bmiHeader.biXPelsPerMeter = 0;
    // ��ֱ�ֱ���(����/��)
    bmiHeader.biYPelsPerMeter = 0;
    // ʹ�õ���ɫ��0Ϊʹ��ȫ����ɫ
    bmiHeader.biClrUsed = 0;
    // ��Ҫ����ɫ����0Ϊ������ɫ����Ҫ
    bmiHeader.biClrImportant = 0;

    // дλͼ��Ϣͷ
    WriteFile(hFile, (LPSTR)&bmiHeader, sizeof(bmiHeader), &bytesWritten, NULL);
    // дλͼ����
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
//������Ĵ������õ�������,���������������������bitmapͼƬ�ı���
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