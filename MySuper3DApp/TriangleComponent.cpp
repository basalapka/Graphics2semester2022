#include "include.h"
#include "TriangleComponent.h"

TriangleComponent::TriangleComponent() {
	parameters.points = nullptr;
	parameters.indeces = nullptr;
	parameters.numPoints = 0;
	parameters.numIndeces = 0;
	compPosition = DirectX::SimpleMath::Vector3(0, 0, 0);

	vertexBC = nullptr;
	pixelBC = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	layout = nullptr;
	vb = nullptr;
	ib = nullptr;
	cb = nullptr;
	rastState = nullptr;

	for (int i = 0; i < 4; i++)
		strides[i] = 0;
	for (int i = 0; i < 4; i++)
		offsets[i] = 0;
}

TriangleComponent::TriangleComponent(TriangleComponentParameters param) {
	parameters.points = param.points;
	parameters.indeces = param.indeces;
	parameters.numPoints = param.numPoints;
	parameters.numIndeces = param.numIndeces;
	compPosition = param.compPosition;

	vertexBC = nullptr;
	pixelBC = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	layout = nullptr;
	vb = nullptr;
	ib = nullptr;
	cb = nullptr;
	rastState = nullptr;

	for (int i = 0; i < 4; i++)
		strides[i] = 0;
	for (int i = 0; i < 4; i++)
		offsets[i] = 0;
}

int TriangleComponent::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, DisplayWin32 DW, HRESULT res) {
	//Компиляция шейдеров
	vertexBC = nullptr;
	ID3DBlob* errorVertexCode = nullptr;
	res = D3DCompileFromFile(L"../Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		//точка входа вершинного шейдера
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexBC,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(DW.get_hWnd(), L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return 0;
	}
	//constData = { 0,0 };
	//Константный буффер
	//cb;
	ConstData constData;
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(constData);
	res = device->CreateBuffer(&constBufDesc, nullptr, &cb);
	if (FAILED(res))
		std::cout<<"Ошибка создания константного буфера";

	//Макрос - устанавливает зеленый цвет на половину экрана
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	//компиляция пиксельного шейдера
	pixelBC;
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"../Shaders/MyVeryFirstShader.hlsl", Shader_Macros /*macros*/, nullptr /*include*/, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC, &errorPixelCode);

	vertexShader;
	pixelShader;
	device->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader);

	device->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader);
	//создание input layout
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};
	device->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	//создание вершинного буффера
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * parameters.numPoints;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = parameters.points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	vb;
	device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);
	//создание буффера
	//parameters.indeces = new int[6] { 0,1,2, 1,0,3 };
	//int indeces[] = { 0,1,2, 1,0,3 };
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * parameters.numIndeces;



	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = parameters.indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	ib;
	device->CreateBuffer(&indexBufDesc, &indexData, &ib);
	strides[0] = 32;
	strides[1] = 32;
	strides[2] = 32;
	strides[3] = 32;
	offsets[0] = 0;
	offsets[1] = 0;
	offsets[2] = 0;
	offsets[3] = 0;
	//размер в байтах на каждую вершину

	//RASTERIZER state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = device->CreateRasterizerState(&rastDesc, &rastState);
	return 0;

	
}
void TriangleComponent::DestroyResourses() {
	if (vertexShader != nullptr)
		vertexShader->Release();
	if (pixelShader != nullptr)
		pixelShader->Release();
	if (layout != nullptr)
		layout->Release();
	if (vb != nullptr)
		vb->Release();
	if (ib != nullptr)
		ib->Release();
	if (cb != nullptr)
		cb->Release();
	if (rastState != nullptr)
		rastState->Release();
}

void TriangleComponent::Update(ID3D11DeviceContext* context, Camera* camera) {
	//std::cout << "asdasf" << std::endl;
	 ////заполнение константного буфера
		//constData.x += x;
		//constData.x += y;
	//context->UpdateSubresource(cb, 0, nullptr, &constData, 0, 0);
	ConstData data;
	data.WorldViewProj = DirectX::SimpleMath::Matrix::CreateTranslation(compPosition) * camera->ViewMatrix * camera->ProjectionMatrix; // получение проекции
	data.WorldViewProj = data.WorldViewProj.Transpose();
	data.World = DirectX::SimpleMath::Matrix::CreateTranslation(parameters.compPosition);
	D3D11_MAPPED_SUBRESOURCE subresourse = {};
	context->Map( // получение указателя на ресурс и запрет доступа GPU к этому ресурсу
		cb,
		0,  // номер подресурса
		D3D11_MAP_WRITE_DISCARD, // получение ресурса для записи
		0, // D3D11_MAP_FLAG_DO_NOT_WAIT
		&subresourse);
	memcpy(
		reinterpret_cast<float*>(subresourse.pData), // куда
		&data, // откуда
		sizeof(constData)); // сколько бай

	context->Unmap(cb, 0);

}

void TriangleComponent::Draw(ID3D11DeviceContext* context) {
	if (parameters.numIndeces != 0)
	{
		context->IASetInputLayout(layout);
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
		context->IASetVertexBuffers(	0,	1,	&vb,	strides,	offsets);
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);
		context->VSSetConstantBuffers(0, 1, &cb);
		context->RSSetState(rastState);
		context->DrawIndexed(
			parameters.numIndeces, // parameters.numIndeces количество отрисовываемых индексов из буфера индексов
			0, // первый индекс для отрисовки
			0);// значение, добавляемое к каждому индексу перед чтением вершины из буфера вершин
	}
	////настройка инпут ассемблера
	//context->IASetInputLayout(TC.layout);
	//context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//context->IASetIndexBuffer(TC.indBuf, DXGI_FORMAT_R32_UINT, 0);
	//context->IASetVertexBuffers(0, 1, &vb, strides, offsets);
	////установка шейдеров
	//context->VSSetShader(TC.vertexShader, nullptr, 0);
	//context->PSSetShader(TC.pixelShader, nullptr, 0);

	//context->VSSetConstantBuffers(0, 1, &cb);
}
