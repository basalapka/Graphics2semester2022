#include "include.h"
#include "Game.h"
//#include "SimpleMath.h"
#include "TriangleComponent.h"

Game::Game() {
	context = nullptr;
	swapChain = nullptr;
	rtv = nullptr;
	debug = nullptr;	
	BGcolor = new float[4]{ 0.0f,0.0f, 0.0f, 0.0f };
	//viewport = nullptr;
}
void Game::Initialize() {
	//CreateTriangle();
}
void Game::Run() {
	Initialize();
	/*inputDevice;
	DW;*/
	inputDevice.Initialize(DW.get_hWnd());
	DW.DisplayWin(&inputDevice);
	//TC;
	int errors = PrepareResources();
	//для каждого сообщения: нажатая кнопка становится символом
	MSG msg = {};
	bool isExitRequested = false;

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
		EndFrame();
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

	}
	DestroyResources();
	//MSG msg = {};
	//bool isExitRequested = false;
	//while (!isExitRequested) { // Цикл до сообщения о выходе от окна или пользователя
	//	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { // Неблокирующий ввод
	//		TranslateMessage(&msg); // Перевод нажатия клавиш в символы
	//		DispatchMessage(&msg); // Обработка сообщения
	//		if (msg.message == WM_QUIT) { // Если было получено сообщение о выходе, в цикл больше входить не надо
	//			isExitRequested = true;
	//		}
	//	}
	//	if (errors == 0)
	//	{
	//		PrepareFrame();
	//		for (int i = 0; i < numVP; i++)
	//		{
	//			PrepareFrameViewport(i);
	//			Update(i);
	//			Draw();
	//		}
	//		EndFrame();
	//	}
	//}
	//DestroyResources();
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


	//Microsoft::WRL::ComPtr<ID3D11Device> device;
	//context;
	//swapChain;


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

	}
	//TriangleComponent
	//TC.Initialize(device, DW, res);
	//указатель на Texture2D
	ID3D11Texture2D* backTex;
	rtv;
	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);	// __uuidof(ID3D11Texture2D) либо айди 
	res = device->CreateRenderTargetView(backTex, nullptr, &rtv);

	//???????????????????????????????
	//context->RSSetState(TC.rastState);

	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->Initialize(device,DW, res);
	}
	return 0;
}
void Game::DestroyResources() {
	/*for (int i = 0; i < Components.size(); i++)
		Components[i]->DestroyResourses();*/
	//КОМПОНЕНТЫ СЮДА
	if (context != nullptr)
	{
		context->ClearState();
		context->Release();
	}
	if (swapChain != nullptr)
		swapChain->Release();
	//if (rtv != nullptr)
	//{
	//	for (int i = 0; i < numVP; i++)
	//	{
	//		if (rtv[i] != nullptr)
	//			rtv[i]->Release();
	//	}
	//}
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

//void Game::PrepareFrameViewport(int nVP) {
//	//context->OMSetRenderTargets(1, &rtv[nVP], depthView); // привязка рендер таргета и буфера глубин к заднему буферу
//	context->RSSetViewports(1, &viewport[nVP]);
//}

void Game::EndFrame() {
	swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0); // замена переднего буфера на задний после отрисовки в задний
}

void Game::Update() {
	for (int i = 0; i < Components.size(); i++)
		Components[i]->Update(context);
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
	rect.positions = new DirectX::XMFLOAT4[rect.numPoints]{
	DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};
	rect.colors = new DirectX::XMFLOAT4[rect.numPoints];
	for (int i = 0; i < rect.numPoints; i++)
		rect.colors[i] = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Components.push_back(new TriangleComponent(rect));*/
	
//}
//вершины с позицией и цветом

