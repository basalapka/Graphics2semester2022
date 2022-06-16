#include "include.h"
#include "TriangleComponent.h"

TriangleComponent::TriangleComponent() {
	parameters.positions = nullptr;
	parameters.colors = nullptr;
	parameters.texcoords = nullptr;
	parameters.indeces = nullptr;
	parameters.numPoints = 0;
	parameters.numIndeces = 0;
	//parameters.textureFileName = nullptr;
	//normals = nullptr;
	compPosition = DirectX::SimpleMath::Vector3(0, 0, 0);

	vertexBC = nullptr;
	pixelBC = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	layout = nullptr;
	/*for (int i = 0; i < 4; i++)
		vBuffers[i] = nullptr;*/
	vb = nullptr;
	ib = nullptr;
	blend = nullptr;
	cb = nullptr;
	//lightBuf = nullptr;
	rastState = nullptr;
	/*factory = nullptr;
	texture = nullptr;
	textureView = nullptr;*/
	sampler = nullptr;

	//sampleMask = 0;
	for (int i = 0; i < 4; i++)
		strides[i] = 0;
	for (int i = 0; i < 4; i++)
		offsets[i] = 0;
	//for (int i = 0; i < 4; i++)
		//blendFactor[i] = 0;
}

TriangleComponent::TriangleComponent(TriangleComponentParameters param) {
	parameters.positions = param.positions;
	parameters.colors = param.colors;
	parameters.texcoords = param.texcoords;
	parameters.indeces = param.indeces;
	parameters.numPoints = param.numPoints;
	parameters.numIndeces = param.numIndeces;
	//parameters.textureFileName = param.textureFileName;
	compPosition = param.compPosition;
	//normals = new DirectX::SimpleMath::Vector4[parameters.numPoints];
	//NormalsCalc(); // считаем нормали для освещения

	vertexBC = nullptr;
	pixelBC = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	layout = nullptr;
	/*for (int i = 0; i < 4; i++)
		vBuffers[i] = nullptr;*/
	vb = nullptr;
	ib = nullptr;
	blend = nullptr;
	cb = nullptr;
	rastState = nullptr;
	/*factory = nullptr;
	texture = nullptr;
	textureView = nullptr;*/
	sampler = nullptr;

	//sampleMask = 0;
	for (int i = 0; i < 4; i++)
		strides[i] = 0;
	for (int i = 0; i < 4; i++)
		offsets[i] = 0;
	//for (int i = 0; i < 4; i++)
		//blendFactor[i] = 0;

	//// Для загрузчика текстур
	//CoInitialize(NULL); // инициализирует библиотеку COM в текущем потоке
	//CoCreateInstance( // создаем COM объект
	//	CLSID_WICImagingFactory2, // CLSID, связанный с данными и кодом, который будет использоваться для создания объекта
	//	NULL, // если NULL, указывает, что объект не создается как часть агрегата
	//	CLSCTX_INPROC_SERVER, // выполняется в том же процессе, что и вызывающая функция
	//	IID_PPV_ARGS(&factory));
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
	constData = { 0,0 };
	//Константный буффер
	cb;
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstData);
	device->CreateBuffer(&constBufDesc, nullptr, &cb);


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
			0,
			//формат
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			//какой буфер
			0,
			//какое смещение в байтах (если 4 флота, то 16)
			0,
			//какой класс слота
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	layout;
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
	vertexData.pSysMem = parameters.positions;
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
	//strides[4] = { 32 };
	//сдвиг
	//offsets[4] = { 0 };


	//RASTERIZER state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	rastState;
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
	for (int i = 0; i < 4; i++)
	{
		/*if (vBuffers[i] != nullptr)
			vBuffers[i]->Release();*/
	}
	if (vb != nullptr)
		vb->Release();
	if (ib != nullptr)
		ib->Release();
	if (blend != nullptr)
		blend->Release();
	if (cb != nullptr)
		cb->Release();
	if (rastState != nullptr)
		rastState->Release();
	/*if (factory != nullptr)
		factory->Release();
	if (texture != nullptr)
		texture->Release();
	if (textureView != nullptr)
		textureView->Release();*/
	if (sampler != nullptr)
		sampler->Release();
}

void TriangleComponent::Update(ID3D11DeviceContext* context) {
	//std::cout << "asdasf" << std::endl;
	 ////заполнение константного буфера
		//constData.x += x;
		//constData.x += y;
	//context->UpdateSubresource(cb, 0, nullptr, &constData, 0, 0);
	D3D11_MAPPED_SUBRESOURCE res = {};
	context->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

	auto dataP = reinterpret_cast<float*>(res.pData);
	memcpy(dataP, &constData, sizeof(ConstData));

	context->Unmap(cb, 0);

}

void TriangleComponent::Draw(ID3D11DeviceContext* context) {
	//parameters.numIndeces != 0
	if (parameters.numIndeces != 0)
	{
		context->IASetInputLayout(layout);
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // список треугольников: задаются вершины каждого треугольника
		// другой вид - лента треугольников (STRIP), когда отрисовка происходит по индексам 0-1-2, 1-2-3, 2-3-4
		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
		context->IASetVertexBuffers(
			0, // первый слот
			1, // 4-количество буферов
			&vb, // буферы вершин
			strides, // шаг вершин для каждого буфера
			offsets); // смещение от начала для каждого буфера
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

//void TriangleComponent::NormalsCalc() {
//	int ind_a, ind_b, ind_c;
//	DirectX::SimpleMath::Vector4 a, b, c, p, q, norm;
//	for (int i = 0; i < parameters.numIndeces; i += 3)
//	{
//		ind_a = parameters.indeces[i];
//		ind_b = parameters.indeces[i + 1];
//		ind_c = parameters.indeces[i + 2];
//
//		a = parameters.positions[ind_a];
//		b = parameters.positions[ind_b];
//		c = parameters.positions[ind_c];
//
//		p = a - b;
//		q = c - b;
//
//		norm.x = p.y * q.z - p.z * q.y;
//		norm.y = p.z * q.x - p.x * q.z;
//		norm.z = p.x * q.y - p.y * q.x;
//		norm.w = 1.0f;
//
//		normals[ind_a] = norm;
//		normals[ind_b] = norm;
//		normals[ind_c] = norm;
//	};
//}
