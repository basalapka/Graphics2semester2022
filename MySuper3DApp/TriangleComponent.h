#pragma once
#include "GameComponent.h"
#include "include.h"


struct TriangleComponentParameters {
	DirectX::XMFLOAT4* points;
	int* indeces;
	int numPoints;
	int numIndeces;
	DirectX::SimpleMath::Vector3 compPosition;
};

struct ConstData
{
	/*float x;
	float y;*/
	DirectX::SimpleMath::Matrix WorldViewProj;
	DirectX::SimpleMath::Matrix World;
};
struct lightData {
	DirectX::SimpleMath::Vector4 Direction;
	DirectX::SimpleMath::Vector4 Color;
	DirectX::SimpleMath::Vector4 ViewerPos;
	//DirectX::SimpleMath::Vector4 normals;
};
class TriangleComponent : public GameComponent {
public:
	TriangleComponentParameters parameters;
	ID3DBlob* vertexBC;
	ID3DBlob* pixelBC;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ConstData constData; // онстантный буфер
	ID3D11Buffer* vb;
	UINT strides[4]; 
	UINT offsets[4]; 
	ID3D11Buffer* ib;
	ID3D11BlendState* blend;
	ID3D11Buffer* cb;
	ID3D11Buffer* lightBuf;
	ID3D11SamplerState* sampler;
	DirectX::SimpleMath::Vector4* normals; // вектор нормалей
	void NormalsCalc();
	ID3D11Buffer* vBuffers[4];

public:
	ID3D11RasterizerState* rastState;
	ID3D11InputLayout* layout; // определение того, как передавать данные вершин
	DirectX::SimpleMath::Vector3 compPosition;
	TriangleComponent();
	TriangleComponent(TriangleComponentParameters param);
	int Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, DisplayWin32 DW, HRESULT res);
	void DestroyResourses();
	void Update(ID3D11DeviceContext* context, Camera* camera) override;
	void Draw(ID3D11DeviceContext* context);
};

