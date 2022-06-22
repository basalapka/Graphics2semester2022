#include "include.h"
#include "TriangleComponent.h"

TriangleComponent::TriangleComponent() {
	parameters.points = nullptr;
	parameters.indeces = nullptr;
	parameters.numPoints = 0;
	parameters.numIndeces = 0;
	compPosition = DirectX::SimpleMath::Vector3(0, 0, 0);

	normals = nullptr;
	vertexBC = nullptr;
	pixelBC = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	layout = nullptr;
	vb = nullptr;
	lightBuf = nullptr;
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
	lightBuf = nullptr;
	rastState = nullptr;
	normals = new DirectX::SimpleMath::Vector4[parameters.numPoints/2];
	NormalsCalc(); // считаем нормали для освещения

	for (int i = 0; i < 4; i++)
		strides[i] = 32;
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

	D3D11_BUFFER_DESC lightBufDesc = {};
	lightBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufDesc.MiscFlags = 0;
	lightBufDesc.StructureByteStride = 0;
	lightBufDesc.ByteWidth = sizeof(lightData);
	res = device->CreateBuffer(&lightBufDesc, nullptr, &lightBuf);
	if (FAILED(res))
		

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
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};
	device->CreateInputLayout(
		inputElements,
		3,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	int size = parameters.numPoints / 2 * 3;
	//создание вершинного буффера
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * (size);


	DirectX::XMFLOAT4* pointsNormals = new DirectX::XMFLOAT4 [size];
	D3D11_SUBRESOURCE_DATA vertexData = {};
//	int temp = 0;
//	for (int i = 0; i < parameters.numPoints; i += 2) {
//		pointsNormals[i] = parameters.points[i]; //вершина
//		pointsNormals[i+1] = parameters.points[i+1]; //цвет
//		//pointsNormals[i+2] = normals[temp]; //нормаль
//		temp++;
//		//std::cout << "x:" << pointsNormals[i+2].x << "y:"<< pointsNormals[i+2].y << "z:" << pointsNormals[i+2].z << std::endl;
//	}
//	for (int i = 0; i < 72; i += 1) {
//	std::cout << "x:" << pointsNormals[i].x << "y:" << pointsNormals[i].y << "z:" << pointsNormals[i].z << std::endl;
//}
	int temp = 0;
int temp1 = 0;
for (int i = 0; i < parameters.numPoints / 2 * 3; i += 3) {
	pointsNormals[i] = parameters.points[temp1]; //вершина
	pointsNormals[i + 1] = parameters.points[temp1 + 1]; //цвет
	temp1 += 2;
	pointsNormals[i + 2] = normals[temp]; //нормаль

	temp++;
	/*std::cout << "x:" << pointsNormals[i+2].x << "y:"<< pointsNormals[i+2].y << "z:" << pointsNormals[i+2].z << std::endl;*/
}
	
	//vertexData.pSysMem = parameters.points;
	vertexData.pSysMem = pointsNormals;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//vb;
	device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);
	//создание буффера
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
	strides[0] = 48;
	strides[1] = 48;
	strides[2] = 48;
	strides[3] = 48;
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
	if (lightBuf != nullptr)
		lightBuf->Release();
	if (rastState != nullptr)
		rastState->Release();
}

void TriangleComponent::Update(ID3D11DeviceContext* context, Camera* camera) {
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

	// заполнения константного буфера для света
	lightData light;
	light.ViewerPos = DirectX::SimpleMath::Vector4(camera->position.x, camera->position.y, camera->position.z, 1.0f);
	light.Direction = DirectX::SimpleMath::Vector4(0.0f, 10.0f, 0.0f, 1.0f); //свет сверху - т.к. ось У - это верх-вниз
	//light.Direction = DirectX::SimpleMath::Vector4(-10.0f, 10.0f, -10.0f, 1.0f);;
	light.Color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	D3D11_MAPPED_SUBRESOURCE subresourse2 = {};
	context->Map( // получение указателя на ресурс и запрет доступа GPU к этому ресурсу
		lightBuf,
		0,  // номер подресурса
		D3D11_MAP_WRITE_DISCARD, // получение ресурса для записи
		0, // D3D11_MAP_FLAG_DO_NOT_WAIT
		&subresourse2);
	memcpy(
		reinterpret_cast<float*>(subresourse2.pData), // куда
		&light, // откуда
		sizeof(lightData)); // сколько байт
	context->Unmap(lightBuf, 0); // вернуть доступ GPU

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
		context->PSSetConstantBuffers(1, 1, &lightBuf);
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
void TriangleComponent::NormalsCalc() {
	int ind_a, ind_b, ind_c;
	DirectX::SimpleMath::Vector4 a, b, c, p, q, norm;
	DirectX::XMFLOAT4* position = new DirectX::XMFLOAT4[parameters.numPoints/2];
	int temp = 0;
	for (int i = 0; i < parameters.numPoints/2; i += 1) {
		position[i] = parameters.points[temp];
		temp += 2;
	}
	for (int i = 0; i < parameters.numIndeces; i += 3)
	{
		ind_a = parameters.indeces[i];
		ind_b = parameters.indeces[i + 1];
		ind_c = parameters.indeces[i + 2];

		a = position[ind_a];
		b = position[ind_b];
		c = position[ind_c];
	
		p = a - b; 
		q = c - b;

		norm.x = p.y * q.z - p.z * q.y;
		norm.y = p.z * q.x - p.x * q.z;
		norm.z = p.x * q.y - p.y * q.x;
		/*norm.x = 0;
		norm.y = 0;
		norm.z = 0;*/
		norm.w = 1.0f;

		/*norm.x *= -1;
		norm.y *= -1;
		norm.z *= -1;*/

		normals[ind_a] = norm;
		normals[ind_b] = norm;
		normals[ind_c] = norm;
	};
}
