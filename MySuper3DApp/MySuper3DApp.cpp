// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "include.h"
#include "DisplayWin32.h"


#pragma pack(push, 4)
//Константный буфер 
struct ConstData
{
	float x;
	float y;
	float dummy0;
	float dummy1;
};
#pragma pack(pop)

ConstData constData = {0,0};


int main()
{
	DisplayWin32 DW;
	DW.DisplayWin();


	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };
	//свап дескриптор
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = DW.get_screenWidth();
	swapDesc.BufferDesc.Height = DW.get_screenHeight();
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = DW.get_hWnd();
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;


	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain,
		&device,
		nullptr,
		&context);
	//если девайс не создан
	if (FAILED(res))
	{

	}
	//указатель на Texture2D
	ID3D11Texture2D* backTex;
	ID3D11RenderTargetView* rtv;
	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);	// __uuidof(ID3D11Texture2D) либо айди 
	res = device->CreateRenderTargetView(backTex, nullptr, &rtv);

	//Компиляция шейдеров
	ID3DBlob* vertexBC = nullptr;
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
	//Макрос - устанавливает зеленый цвет на половину экрана
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };
	
	//компиляция пиксельного шейдера
	ID3DBlob* pixelBC;
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"../Shaders/MyVeryFirstShader.hlsl", Shader_Macros /*macros*/, nullptr /*include*/, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC, &errorPixelCode);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
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

	ID3D11InputLayout* layout;
	device->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);
	//вершины с позицией и цветом
	DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};

	//создание вершинного буффера
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ID3D11Buffer* vb;
	device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);
	//создание буффера
	int indeces[] = { 0,1,2, 1,0,3 };
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	ID3D11Buffer* ib;
	device->CreateBuffer(&indexBufDesc, &indexData, &ib);

	//размер в байтах на каждую вершину
	UINT strides[] = { 32 };
	//сдвиг
	UINT offsets[] = { 0 };


	//RASTERIZER state
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rastState;
	res = device->CreateRasterizerState(&rastDesc, &rastState);

	context->RSSetState(rastState);

//Константный буффер
	ID3D11Buffer* cb;
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstData);
	device->CreateBuffer(&constBufDesc, nullptr, &cb);

	//время
	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;

	//для каждого сообщения: нажатая кнопка становится символом
	MSG msg = {};
	bool isExitRequested = false;
	while (!isExitRequested) {
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}
		//убирает шейдеры и т.д.
		context->ClearState();

		context->RSSetState(rastState);
		//где можно будет рисовать
		D3D11_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(DW.get_screenWidth());
		viewport.Height = static_cast<float>(DW.get_screenHeight());
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;
		
		context->RSSetViewports(1, &viewport);
		//настройка инпут ассемблера
		context->IASetInputLayout(layout);
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
		context->IASetVertexBuffers(0, 1, &vb, strides, offsets);
		//установка шейдеров
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);

		context->VSSetConstantBuffers(0, 1, &cb);
		//подсчет времени между кадрами и фпс
		auto	curTime = std::chrono::steady_clock::now();
		float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
		PrevTime = curTime;

		totalTime += deltaTime;
		frameCount++;

		if (totalTime > 1.0f) {
			float fps = frameCount / totalTime;

			totalTime -= 1.0f;

			WCHAR text[256];
			swprintf_s(text, TEXT("FPS: %f"), fps);
			SetWindowText(DW.get_hWnd(), text);

			frameCount = 0;
		}
		//обрабатывание нажатых клавиш
		
		//if (pressedKeys.count(37)) {
		//	//std::cout << "37 is working";
		//		constData.x -= 0.01f;
		//}
		//if (pressedKeys.count(39))
		//	constData.x += 0.01f;
		//if (pressedKeys.count(38))
		//	constData.y += 0.01f;
		//if (pressedKeys.count(40))
		//	constData.y -= 0.01f;

		//context->UpdateSubresource(cb, 0, nullptr, &constData, 0, 0);
		D3D11_MAPPED_SUBRESOURCE res = {};
		context->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

		auto dataP = reinterpret_cast<float*>(res.pData);
		memcpy(dataP, &constData, sizeof(ConstData));

		context->Unmap(cb, 0);



		context->OMSetRenderTargets(1, &rtv, nullptr);
		//очистка бэкбуффера , красный цвет на фоне
		float color[] = { totalTime, 0.1f, 0.1f, 1.0f };
		context->ClearRenderTargetView(rtv, color);
		//функция отрисовки
		context->DrawIndexed(6, 0, 0);

		context->OMSetRenderTargets(0, nullptr, nullptr);
		//
		swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
