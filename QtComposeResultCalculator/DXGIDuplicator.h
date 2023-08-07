// DXGIDuplicator.h

#include <d3d11.h>
#include <dxgi1_2.h>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class DXGIDuplicator
{
public:
    DXGIDuplicator();
    ~DXGIDuplicator();

    bool InitD3D11Device();

    bool InitDuplication();

    bool GetDesktopFrame(ID3D11Texture2D** texture);

    // ��Ԫ��������main����������õ�����Ҫ������˽�г�Ա
    friend void SaveDesktopImage(std::string filename, ID3D11Texture2D* texture, DXGIDuplicator* duplicator);

private:
    ID3D11Device* device_ = nullptr;
    ID3D11DeviceContext* deviceContext_ = nullptr;
    IDXGIOutputDuplication* duplication_ = nullptr;
};