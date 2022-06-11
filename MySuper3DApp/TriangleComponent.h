#pragma once
#include "GameComponent.h"
#include "include.h"

struct TriangleComponentParameters {
	DirectX::SimpleMath::Vector4* positions;
	DirectX::SimpleMath::Vector4* colors;
	DirectX::SimpleMath::Vector4* texcoords;
	int* indeces;
	int numPoints;
	int numIndeces;
	//LPCWSTR textureFileName; // �������� ����� � ���������
	DirectX::SimpleMath::Vector3 compPosition;
};

//struct constData
//{
//	DirectX::SimpleMath::Matrix WorldViewProj;
//	DirectX::SimpleMath::Matrix World;
//};
struct �onstData
{
	float x;
	float y;
	float dummy0;
	float dummy1;
};

class TriangleComponent{
public:
	TriangleComponentParameters parameters;
	ID3DBlob* vertexBC; // ������� ���� ���, ��������� ���������� ���������� ����� � ���������� ��������
	ID3DBlob* pixelBC; // ������� ���� ���, ��������� ���������� ���������� ����� � ���������� ��������
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	�onstData constData;
	ID3D11Buffer* vb;
	UINT strides[4]; // ��� ������ ��� ������� ������
	UINT offsets[4]; // �������� �� ������ ��� ������� ������
	ID3D11Buffer* ib;
	ID3D11BlendState* blend;
	ID3D11Buffer* cb;
	//float blendFactor[4];
	//UINT sampleMask;
	//IWICImagingFactory2* factory; // ��������� ���������� �������
	//ID3D11Texture2D* texture; // ��������
	//ID3D11ShaderResourceView* textureView;
	ID3D11SamplerState* sampler;

	//DirectX::SimpleMath::Vector4* normals; // ������ ��������
	//void NormalsCalc();
public:
	ID3D11RasterizerState* rastState;
	ID3D11InputLayout* layout; // ����������� ����, ��� ���������� ������ ������, ������� ��������� � ������, �� ���� �������� ����� ������������ ���������
	DirectX::SimpleMath::Vector3 compPosition;
	TriangleComponent();
	TriangleComponent(TriangleComponentParameters param);
	int Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, DisplayWin32 DW, HRESULT res);
	void DestroyResourses();
	void Update(ID3D11DeviceContext* context);
	void Draw(ID3D11DeviceContext* context);
};

