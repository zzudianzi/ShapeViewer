#pragma once

#include "d3dApp.h"
#include <DirectXMath.h>
#include "MathHelper.h"
#include "UploadBuffer.h"

#include "Polyline.h"

struct ObjectConstants;

class BoxApp : public D3DApp
{
  public:
    BoxApp(winrt::com_ptr<ISwapChainPanelNative> panelNative);
    ~BoxApp() override;

    bool InitDirect3D() override;

  protected:
    void OnResize(float width, float height) override;
    void Update() override;
    void Draw() override;

    void OnMouseDown(WPARAM btnState, int x, int y) override;
    void OnMouseUp(WPARAM btnState, int x, int y) override;
    void OnMouseMove(WPARAM btnState, int x, int y) override;

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShaderAndInputLayout();
    void BuildBoxGeometry();
    void BuildPSO();


    void UpdateGeometry(const ::ShapeViewer::Polyline& polyline) override;

  private:
    POINT _LastMousePos;
    float _Theta = 1.5f * DirectX::XM_PI;
    float _Phi = DirectX::XM_PIDIV4;
    float _Radius = 5.f;

    DirectX::XMFLOAT4X4 _World = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 _View = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 _Proj = MathHelper::Identity4x4();

    std::unique_ptr<UploadBuffer<ObjectConstants>> _ObjectCB;

    winrt::com_ptr<ID3D12DescriptorHeap> _CBVHeap;

    winrt::com_ptr<ID3D12RootSignature> _RootSignature;

    winrt::com_ptr<ID3DBlob> _VSByteCode;
    winrt::com_ptr<ID3DBlob> _PSByteCode;

    std::vector<D3D12_INPUT_ELEMENT_DESC> _InputLayout;

    std::unique_ptr<MeshGeometry> _BoxGeometry;

    winrt::com_ptr<ID3D12PipelineState> _PSO;
};
