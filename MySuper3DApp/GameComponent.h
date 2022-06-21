#pragma once
#include "include.h"
#include "DisplayWin32.h"
using namespace DirectX;

class GameComponent {
public:
	virtual int Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, DisplayWin32 DW, HRESULT res) { return 0; };
	//virtual int LoadTextureFromFile(Microsoft::WRL::ComPtr<ID3D11Device> device, ID3D11DeviceContext* context, bool generateMips, bool useSrgb, UINT frameIndex) { return 0; };
	virtual void DestroyResourses() {};
	virtual void Update(ID3D11DeviceContext* context, Camera* camera) =0;
	virtual void Draw(ID3D11DeviceContext* context) {};
};
