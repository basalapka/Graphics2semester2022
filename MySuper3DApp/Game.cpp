#include "include.h"
#include "Game.h"
#include "TriangleComponent.h"

Game::Game() {
	context = nullptr;
	swapChain = nullptr;
	rtv = nullptr;
	debug = nullptr;	
	BGcolor = new float[4]{ 0.0f,0.0f, 0.0f, 0.0f };
}
void Game::Initialize() {

}
void Game::Run() {
	Initialize();
	inputDevice.Initialize(DW.get_hWnd());
	DW.DisplayWin(&inputDevice);
	//TC;
	int errors = PrepareResources();
	//для каждого сообщения: нажатая кнопка становится символом
	MSG msg = {};
	bool isExitRequested = false;
	camera.push_back(new Camera);
	camera.back()->Initialize(DirectX::SimpleMath::Vector3(0.0f, 0.5f, 2.0f), (1.57 / 2), (1.57 / 2), DW.get_screenWidth(), DW.get_screenHeight(), &inputDevice);

	while (!isExitRequested) {
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			// If windows signals to end the application then exit out.
			if (msg.message == WM_QUIT) {
				isExitRequested = true;
			}
		}
		//context->RSSetState(TC.rastState);
		//функция отрисовки
		//TC.Draw(context);
		//context->DrawIndexed(6, 0, 0);
		//context->OMSetRenderTargets(0, nullptr, nullptr);
		//
		//swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
		PrepareFrame();
			context->RSSetViewports(1, &viewport);
			context->OMSetRenderTargets(1, &rtv, nullptr);
			Update();
			Draw();
		swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);

	}
	DestroyResources();
}

int Game::PrepareResources() {
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

	//где можно будет рисовать
	viewport = {};
	viewport.Width = static_cast<float>(DW.get_screenWidth());
	viewport.Height = static_cast<float>(DW.get_screenHeight());
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	HRESULT res = D3D11CreateDeviceAndSwapChain(
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
		std::cout << "Девайс не создан :(" << std::endl;
	}
	//TriangleComponent
	//TC.Initialize(device, DW, res);
	//указатель на Texture2D
	ID3D11Texture2D* backTex;
	rtv;
	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);	// __uuidof(ID3D11Texture2D) либо айди 
	res = device->CreateRenderTargetView(backTex, nullptr, &rtv);

	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->Initialize(device,DW, res);
	}
	return 0;
}
void Game::DestroyResources() {
	for (int i = 0; i < Components.size(); i++)
		Components[i]->DestroyResourses();
	//КОМПОНЕНТЫ СЮДА
	if (context != nullptr)
	{
		context->ClearState();
		context->Release();
	}
	if (swapChain != nullptr)
		swapChain->Release();
	if (device != nullptr)
		device->Release();
	if (debug != nullptr)
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}

void Game::PrepareFrame() {
	////время
	//std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	//float totalTime = 0;
	//unsigned int frameCount = 0;
	auto curTime = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prevTime).count() / 1000000.0f;
	prevTime = curTime;
	totalTime += deltaTime;
	frameCount++;
	if (totalTime > 1.0f) { // подсчет кадров в секунду
		float fps = frameCount / totalTime;
		totalTime = 0.0f;
		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(DW.get_hWnd(), text);
		frameCount = 0;
	}
	context->ClearState();
	//context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//очистка бэкбуффера , цвет на фоне
	
}

void Game::Update() {
	camera.at(0)->Update(deltaTime, DW.get_screenWidth(), DW.get_screenHeight());
	for (int i = 0; i < Components.size(); i++)
		Components[i]->Update(context, camera.at(0));
}

void Game::Draw() {
	//TC.Draw(context);
	context->ClearRenderTargetView(rtv, BGcolor);
	for (int i = 0; i < Components.size(); i++)
		Components[i]->Draw(context);
}

//void Game::CreateTriangle() {
	/*TriangleComponentParameters rect;
	rect.numPoints = 8;
	rect.numIndeces = 6;
	rect.points = new DirectX::XMFLOAT4[rect.numPoints]{
	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};
	Components.push_back(new TriangleComponent(rect));*/


