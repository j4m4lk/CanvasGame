#include "MainRender.h"
#include <Windows.h>


using namespace DirectX;
using namespace std;



MainRender::MainRender() :
	mWidth(0),
	mHeight(0),
	Resetclicked(false),
	inputManager(GetActiveWindow()) // Initialize the InputManager in the member initializer list with the active window handle
{
}


MainRender::~MainRender()
{
}


POINT GetCursorPosition()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	return cursorPos;
}

void MainRender::SetWindow(const HWND& Wnd)
{
	mWnd = Wnd;
}


/// Compiles shader from files

HRESULT MainRender::CompileShaderFromFile(const WCHAR* const szFileName, const LPCSTR szEntryPoint, const LPCSTR szShaderModel, ID3DBlob** const ppBlobOut) const
{
	HRESULT hr = static_cast<HRESULT>(0L);

	const DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;


	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, const_cast<ID3DBlob**>(ppBlobOut), &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return static_cast<HRESULT>(0L);
}


HRESULT MainRender::InitDXDevice()
{
	HRESULT hr = static_cast<HRESULT>(0L);

	RECT rc;
	GetClientRect(mWnd, &rc);
	mWidth = rc.right - rc.left;
	mHeight = rc.bottom - rc.top;

	const UINT createDeviceFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		mDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &mD3dDevice, &mFeatureLevel, &mImmediateContext);

		if (hr == static_cast<HRESULT>(0x80070057L))
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &mD3dDevice, &mFeatureLevel, &mImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = mD3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return hr;

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	// DirectX 11.1 or later
	hr = mD3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mD3dDevice1));
	if (SUCCEEDED(hr))
	{
		static_cast<void>(mImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mImmediateContext1)));
	}

	DXGI_SWAP_CHAIN_DESC1 sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Width = mWidth;
	sd.Height = mHeight;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;



	hr = dxgiFactory2->CreateSwapChainForHwnd(mD3dDevice, mWnd, &sd, nullptr, nullptr, &mSwapChain1);
	if (SUCCEEDED(hr))
	{
		hr = mSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&mSwapChain));
	}

	dxgiFactory2->Release();
	
	

	//Disable full screen
	dxgiFactory->MakeWindowAssociation(mWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = mD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = mWidth;
	descDepth.Height = mHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mD3dDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mD3dDevice->CreateDepthStencilView(mDepthStencil, &descDSV, &mDepthStencilView);
	if (FAILED(hr))
		return hr;

	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(mWidth);
	vp.Height = static_cast<float>(mHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mImmediateContext->RSSetViewports(1, &vp);

	// Set primitive topology
	mImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	mBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	mBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufferDesc.CPUAccessFlags = 0;
	hr = mD3dDevice->CreateBuffer(&mBufferDesc, nullptr, &mConstantBuffer);
	if (FAILED(hr))
		return hr;

	//Set constant buffers
	mImmediateContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);
	mImmediateContext->PSSetConstantBuffers(0, 1, &mConstantBuffer);

	//Create sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	hr = mD3dDevice->CreateSamplerState(&sampDesc, &mSampler);
	if (FAILED(hr))
		return hr;

	//Set sampler state
	mImmediateContext->PSSetSamplers(0, 1, &mSampler);

	//Create blend state for alpha blending
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = mD3dDevice->CreateBlendState(&blendDesc, &mBlendStateAlpha);
	if (FAILED(hr))
		return hr;

	//Create default blend state for disabling blend
	blendDesc.RenderTarget[0].BlendEnable = FALSE;

	hr = mD3dDevice->CreateBlendState(&blendDesc, &mBlendStateDisable);
	if (FAILED(hr))
		return hr;


	//initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(mWnd);
	ImGui_ImplDX11_Init(mD3dDevice, mImmediateContext);
	ImGui::StyleColorsDark();
	// imgui 

	return static_cast<HRESULT>(0L);
}





//Listen for Messages to register inputs
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND  hwnd, UINT  umsg, WPARAM wparam, LPARAM  lparam);
LRESULT CALLBACK WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	//static int mouseX, mouseY; // Declare the variables outside the switch statement


	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
		//this is moved now will need to remove this 
	//case WM_LBUTTONDOWN:
	//	// Left mouse button is clicked
	//	OutputDebugStringA("Left mouse button down\n");
	//	break;

	//case WM_LBUTTONUP:
	//	// Left mouse button is released
	//	OutputDebugStringA("Left mouse button up\n");
	//	break;

	//case WM_MOUSEMOVE:
	//	// Mouse moved
	//	// Extract mouse coordinates from the message parameters
	//	mouseX = GET_X_LPARAM(lParam);
	//	mouseY = GET_Y_LPARAM(lParam);
	//	// Output mouse coordinates
	//	char buffer[256];
	//	sprintf_s(buffer, "Mouse moved to (%d, %d)\n", mouseX, mouseY);
	//	OutputDebugStringA(buffer);
	//	break;

	
	break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


HRESULT MainRender::Render(const vector<GameObject>& entities, const Camera* const camera, const float& time)
{
	HRESULT hr = static_cast<HRESULT>(0L);

	
	cb1.time = time;

	// Clear the back buffer
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, Colors::LightSkyBlue);

	// Clear the depth buffer to 1.0 (max depth)
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	SetCamera(camera);
	
	for (const auto& entity : entities)
	{
		
	}
	
	for (const auto& entity : entities)
	{
		for (const auto& shape : entity.Shapes())
		{
			
			hr = Shader(shape);
			if (FAILED(hr))
			{
				return hr;
			}
			
			
			
			
			hr = VertexIndex(shape);
			if (FAILED(hr))
			{
				return hr;
			}
			
			XMStoreFloat4x4(&cb1.mWorld, XMMatrixTranspose(XMLoadFloat4x4(&shape.Transform())));
			XMStoreFloat4x4(&cb1.mWorldInverse, XMMatrixInverse(NULL, XMLoadFloat4x4(&cb1.mWorld)));
			mImmediateContext->UpdateSubresource(mConstantBuffer, 0, nullptr, &cb1, 0, 0);

		
			

			
			if (shape.Instances().size() > 0)
			{
				mImmediateContext->DrawIndexedInstanced(shape.Indices().size(), shape.Instances().size(), 0, 0, 0);
			}
			
			else
			{
				mImmediateContext->DrawIndexed(shape.Indices().size(), 0, 0);
			}
		}
	}


	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();	
	ImGui::Begin("Missle Somulation");
	
	//Make keys and then assign them the values for the fucntions
	ImGui::Text(" Simulation Controls ");
	if (ImGui::Button("Reset Scene")) {
		Resetclicked = true;
	}
	//ImGui::Text(" Time scale : " + to_string(timescale).c_str());
	if (ImGui::Button("cam1")) {
	//	cam = 1;
	}
	if (ImGui::Button("cam2")) {
	//	cam = 2;
	}
	if (ImGui::Button("cam3")) {
		//activeCam = &cameras[2];
	}
	if (ImGui::Button("cam4")) {
		//activeCam = &cameras[3];
	}

	ImGui::Text("Timescale") ;

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());	
	
	//Everything should be called before this to render them 

	mSwapChain->Present(1, 0);


	return hr;
}





void MainRender::SetCamera(const Camera* const camera)
{
	XMStoreFloat4x4(&cb1.mView, XMMatrixTranspose(XMLoadFloat4x4(&camera->View())));
	XMStoreFloat4x4(&cb1.mProjection, XMMatrixTranspose(XMLoadFloat4x4(&camera->Projection())));
	cb1.vCameraPos = XMFLOAT4(camera->Position().x, camera->Position().y, camera->Position().z, 0.0f);
}

HRESULT MainRender::VertexIndex(const VerticeShapes& shape)
{
	HRESULT hr = static_cast<HRESULT>(0L);
	if (mVertexBuffers.count(shape.Geometry()) == 0)
	{
		
		ZeroMemory(&mBufferDesc, sizeof(mBufferDesc));
		mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mBufferDesc.ByteWidth = sizeof(SimpleVertex) * shape.Vertices().size();
		mBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE;

		mBufferDesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &(shape.Vertices()[0]);
		ID3D11Buffer* vertexBuffer = nullptr;
		hr = mD3dDevice->CreateBuffer(&mBufferDesc, &InitData, &vertexBuffer);
		if (FAILED(hr))
		{
			return hr;
		}
	
		mVertexBuffers.insert(pair<MeshType, ID3D11Buffer*>(shape.Geometry(), vertexBuffer));

		const UINT stride = sizeof(SimpleVertex);
		const UINT offset = 0;
		mImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	}
	else
	{
		
		const UINT stride = sizeof(SimpleVertex);
		const UINT offset = 0;
		mImmediateContext->IASetVertexBuffers(0, 1, &mVertexBuffers[shape.Geometry()], &stride, &offset);
	}

	if (mIndexBuffers.count(shape.Geometry()) == 0)
	{
		
		mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mBufferDesc.ByteWidth = sizeof(WORD) * shape.Indices().size();
		mBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		mBufferDesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &(shape.Indices()[0]);
		ID3D11Buffer* indexBuffer = nullptr;
		hr = mD3dDevice->CreateBuffer(&mBufferDesc, &InitData, &indexBuffer);
		if (FAILED(hr))
		{
			return hr;
		}		
		mIndexBuffers.insert(pair<MeshType, ID3D11Buffer*>(shape.Geometry(), indexBuffer));		
		mImmediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}
	else
	{		
		mImmediateContext->IASetIndexBuffer(mIndexBuffers[shape.Geometry()], DXGI_FORMAT_R16_UINT, 0);
	}


	if (shape.Instances().size() > 0)
	{	
		if (mInstanceBuffers.count(shape.Name()) == 0)
		{
			
			ZeroMemory(&mBufferDesc, sizeof(mBufferDesc));
			mBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			mBufferDesc.ByteWidth = sizeof(InstanceData) * shape.Instances().size();
			mBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			mBufferDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = &(shape.Instances()[0]);
			ID3D11Buffer* instanceBuffer = nullptr;
			hr = mD3dDevice->CreateBuffer(&mBufferDesc, &InitData, &instanceBuffer);
			if (FAILED(hr))
			{
				return hr;
			}		
			mInstanceBuffers.insert(pair<string, ID3D11Buffer*>(shape.Name(), instanceBuffer));			
			const UINT stride = sizeof(InstanceData);
			const UINT offset = 0;
			mImmediateContext->IASetVertexBuffers(1, 1, &instanceBuffer, &stride, &offset);
		}
		else
		{			
			const UINT stride = sizeof(InstanceData);
			const UINT offset = 0;
			mImmediateContext->IASetVertexBuffers(1, 1, &mInstanceBuffers[shape.Name()], &stride, &offset);
		    mImmediateContext->UpdateSubresource(mInstanceBuffers[shape.Name()], 0, nullptr, &(shape.Instances()[0]), 0, 0);
		}
	}

	return hr;
}


// Initializes windows and its parameters
HRESULT InitWindow(const HINSTANCE hInstance, const int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, reinterpret_cast<LPCTSTR>(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"Missle Simulation";
	wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<LPCTSTR>(IDI_ICON1));
	if (!RegisterClassEx(&wcex))
		return static_cast<HRESULT>(0x80004005L);

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1600, 900 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"Missle Simulation", L"Missle Simulation",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!g_hWnd)
		return static_cast<HRESULT>(0x80004005L);

	ShowWindow(g_hWnd, nCmdShow);

	return static_cast<HRESULT>(0L);
}

HRESULT MainRender::Shader(const VerticeShapes& shape)
{
	HRESULT hr = static_cast<HRESULT>(0L);

	
	if (mVertexShaders.count(shape.Shader()) == 0)
	{
		
		ID3DBlob* pVSBlob = nullptr;
		hr = CompileShaderFromFile(shape.Shader().c_str(), "VS", "vs_4_0", &pVSBlob);
		if (FAILED(hr))
		{
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			return hr;
		}		
		ID3D11VertexShader* vertexShader = nullptr;
		hr = mD3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &vertexShader);
		if (FAILED(hr))
		{
			pVSBlob->Release();
			return hr;
		}		
		mVertexShaders.insert(pair<wstring, ID3D11VertexShader*>(shape.Shader(), vertexShader));	
		mImmediateContext->VSSetShader(vertexShader, nullptr, 0);		
		if (mLayouts.count(shape.Shader()) == 0)
		{			
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "INSTANCEID", 0, DXGI_FORMAT_R32_SINT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "INSTANCEPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "CUBEID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // new field
				{ "COLORTOAPPLY", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 28, D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // new field
			};

			// ...

			// then, you use layout to create the input layout

			UINT numElements = ARRAYSIZE(layout);

			
			ID3D11InputLayout* vertexLayout = nullptr;
			hr = mD3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
				pVSBlob->GetBufferSize(), &vertexLayout);
			pVSBlob->Release();
			if (FAILED(hr))
				return hr;
			
			mLayouts.insert(pair<wstring, ID3D11InputLayout*>(shape.Shader(), vertexLayout));			
			mImmediateContext->IASetInputLayout(vertexLayout);
		}
		else
		{		
			mImmediateContext->IASetInputLayout(mLayouts[shape.Shader()]);
		}
	}
	else
	{		
		mImmediateContext->VSSetShader(mVertexShaders[shape.Shader()], nullptr, 0);
	}

	
	if (mPixelShaders.count(shape.Shader()) == 0)
	{
	
		ID3DBlob* pPSBlob = nullptr;
		hr = CompileShaderFromFile(shape.Shader().c_str(), "PS", "ps_4_0", &pPSBlob);
		if (FAILED(hr))
		{
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			return hr;
		}

		
		ID3D11PixelShader* pixelShader = nullptr;
		hr = mD3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pixelShader);
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;

		
		mPixelShaders.insert(pair<wstring, ID3D11PixelShader*>(shape.Shader(), pixelShader));

	
		mImmediateContext->PSSetShader(pixelShader, nullptr, 0);
	}
	else
	{
	
		mImmediateContext->PSSetShader(mPixelShaders[shape.Shader()], nullptr, 0);
	}

	return static_cast<HRESULT>(0L);
}


void MainRender::CleanUpDevice()
{
	if (mImmediateContext) mImmediateContext->ClearState();
	
	for (const auto& buffer : mVertexBuffers)
	{
		buffer.second->Release();
	}
	
	for (const auto& buffer : mIndexBuffers)
	{
		buffer.second->Release();
	}
	
	for (const auto& buffer : mInstanceBuffers)
	{
		buffer.second->Release();
	}

	for (const auto& shader : mVertexShaders)
	{
		shader.second->Release();
	}

	for (const auto& shader : mPixelShaders)
	{
		shader.second->Release();
	}

	for (const auto& texture : mTextures)
	{
		texture.second->Release();
	}

	for (const auto& layout : mLayouts)
	{
		layout.second->Release();
	}

	if (mConstantBuffer) mConstantBuffer->Release();
	if (mDepthStencil) mDepthStencil->Release();
	if (mDepthStencilView) mDepthStencilView->Release();
	if (mRenderTargetView) mRenderTargetView->Release();
	if (mSwapChain1) mSwapChain1->Release();
	if (mSwapChain) mSwapChain->Release();
	if (mImmediateContext1) mImmediateContext1->Release();
	if (mImmediateContext) mImmediateContext->Release();
	if (mD3dDevice1) mD3dDevice1->Release();
	if (mD3dDevice) mD3dDevice->Release();
	if (mSampler) mSampler->Release();
	if (mRasteriserStateNoCull) mRasteriserStateNoCull->Release();
	if (mRasteriserStateBackCull) mRasteriserStateBackCull->Release();
	if (mDepthStencilLessThanEqual) mDepthStencilLessThanEqual->Release();
	if (mBlendStateAlpha) mBlendStateAlpha->Release();
	if (mBlendStateDisable) mBlendStateDisable->Release();

}


const int& MainRender::Width() const
{
	return mWidth;
}

const int& MainRender::Height() const
{
	return mHeight;
}

int WINAPI wWinMain(_In_ const HINSTANCE hInstance, _In_opt_ const HINSTANCE hPrevInstance, _In_ const LPWSTR lpCmdLine, _In_ const int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	
	MainRender renderer;
	renderer.SetWindow(g_hWnd);

	//Initisialises DirectX device, and cleans up device if there's a failure
	if (FAILED(renderer.InitDXDevice()))
	{
		renderer.CleanUpDevice();
		return 0;
	}

	//Initialises the scene with all required entities, cameras and lights
	MainRender gm(renderer.Width(), renderer.Height());
	gm.InitScene();

	//Main message loop
	MSG msg = { 0 };
	auto previousTime = chrono::high_resolution_clock::now();
	auto startTime = chrono::high_resolution_clock::now();
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Calculates delta time
			auto currentTime = chrono::high_resolution_clock::now();
			const auto elapsedTimeTotal = currentTime - startTime;
			const auto elapsedTime = currentTime - previousTime;
			const float dt = (elapsedTime.count() / 1e+9);
			const float time = (elapsedTimeTotal.count() / 1e+9);

			//Update Render Loop
			gm.Update(dt);

			if (gm.ExitApplication())
			{
				renderer.CleanUpDevice();
				return 0;
			}
			if (FAILED(renderer.Render(gm.Entities(), gm.ActiveCamera(), time)))
			{
				renderer.CleanUpDevice();
				return 0;
			}
			previousTime = currentTime;
		}
		if (renderer.getClicked())
		{
			gm.ResetScene();
			renderer.setReset(false);
			
		}
		renderer.camsetting(renderer.cam);
	}

	//Cleanup device on exit
	renderer.CleanUpDevice();

	return static_cast<int>(msg.wParam);
}

void MainRender::camsetting(int ch)
{
	switch (ch) {
	case 1:
		activeCam = &cameras[0];
	
	case 2:
		activeCam = &cameras[1];
}

}

void MainRender::CheckInput(const float& dt)
{
	//Assign the state of keyboard to start 
	const auto kb = keyboard->GetState();


	if (kb.Escape)
	{
		isExit = true;
	}
	//Camera movement 
	if (kb.W && activeCam->AllowMovement() && kb.LeftControl)
	{
		activeCam->MoveCamera(activeCam->Forward(), dt);
	}
	if (kb.A && activeCam->AllowMovement() && kb.LeftControl)
	{
		activeCam->MoveCamera(XMFLOAT3(-activeCam->Right().x, -activeCam->Right().y, -activeCam->Right().z), dt);
	}
	if (kb.S && activeCam->AllowMovement() && kb.LeftControl)
	{
		activeCam->MoveCamera(XMFLOAT3(-activeCam->Forward().x, -activeCam->Forward().y, -activeCam->Forward().z), dt);
	}
	if (kb.D && activeCam->AllowMovement() && kb.LeftControl)
	{
		activeCam->MoveCamera(activeCam->Right(), dt);
	}
	if (kb.PageUp && activeCam->AllowMovement() && kb.LeftControl)
	{
		activeCam->MoveCamera(activeCam->Up(), dt);
	}
	if (kb.PageDown && activeCam->AllowMovement() && kb.LeftControl)
	{
		activeCam->MoveCamera(XMFLOAT3(-activeCam->Up().x, -activeCam->Up().y, -activeCam->Up().z), dt);
	}

	//Camera rotation 
	if (kb.W && activeCam->AllowMovement() && !kb.LeftControl)
	{
		activeCam->RotateCamera(XMFLOAT3(-XMConvertToRadians(5), 0, 0), dt);
	}
	if (kb.A && activeCam->AllowMovement() && !kb.LeftControl)
	{
		activeCam->RotateCamera(XMFLOAT3(0, -XMConvertToRadians(5), 0), dt);
	}
	if (kb.S && activeCam->AllowMovement() && !kb.LeftControl)
	{
		activeCam->RotateCamera(XMFLOAT3(XMConvertToRadians(5), 0, 0), dt);
	}
	if (kb.D && activeCam->AllowMovement() && !kb.LeftControl)
	{
		activeCam->RotateCamera(XMFLOAT3(0, XMConvertToRadians(5), 0), dt);
	}


	//Cameras selection

	if (kb.F1)
	{
		activeCam = &cameras[0];
	}
	if (kb.F2)
	{
		activeCam = &cameras[1];
	}
	if (kb.F3)
	{
		activeCam = &cameras[2];
	}
	if (kb.F4)
	{
		activeCam = &cameras[3];
	}
	if (kb.F5)
	{
		activeCam = &cameras[4];
	}
	if (kb.F6)
	{
		//setwireframe 
	}
	if (kb.OemPeriod && kb.LeftShift && !isRocketLaunched)
	{
		
	}
	if (kb.OemComma && kb.LeftShift && !isRocketLaunched)
	{
		
	}
	if (kb.F11)
	{
		isRocketLaunched = true;
	}
	if (kb.R)
	{
		ResetScene();
	}
	if (kb.T)
	{
		//timescale -= 0.05f;
		//// to avoid goin in reverse 
		//if (timescale <= 1) timescale = 1;
	}
	if (kb.LeftShift && kb.T)
	{
		timescale += 0.5f;
	}
}


void MainRender::CreateCameras()
{

	const float fov = XMConvertToRadians(60);
	const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	//camera 1 Position
	Camera newCamera(true, XMFLOAT3(-10.0f, 20.0f, 8), XMFLOAT3(XMConvertToRadians(90), 0, 0), fov, aspectRatio, 0.1f, 100.0f, 1.0f, 10.0f);
	cameras.push_back(newCamera);

	

	//Assign the camera pointers to cameras;
	activeCam = &cameras[0];
	
}


bool AABBIntersect(const AABB& box, const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDir)
{
	DirectX::XMFLOAT3 dir = DirectX::XMFLOAT3(DirectX::XMVectorGetX(rayDir), DirectX::XMVectorGetY(rayDir), DirectX::XMVectorGetZ(rayDir));

	float tmin = (box.center.x - box.extents.x - DirectX::XMVectorGetX(rayOrigin)) / dir.x;
	float tmax = (box.center.x + box.extents.x - DirectX::XMVectorGetX(rayOrigin)) / dir.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (box.center.y - box.extents.y - DirectX::XMVectorGetY(rayOrigin)) / dir.y;
	float tymax = (box.center.y + box.extents.y - DirectX::XMVectorGetY(rayOrigin)) / dir.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (box.center.z - box.extents.z - DirectX::XMVectorGetZ(rayOrigin)) / dir.z;
	float tzmax = (box.center.z + box.extents.z - DirectX::XMVectorGetZ(rayOrigin)) / dir.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return ((tmin < 0) && (tmax > 0)) || (tmin < 0);
}



void MainRender::CreateVoxels()
{

	std::vector<InstanceData> instances;

	int id = 0;  // Initialize cube ID counter

	for (int y = 0; y < 2; y++) {
		for (int z = 0; z < 10; z++) {
			for (int x = 0; x < 10; x++) {
				InstanceData instance;
				instance.Pos = DirectX::XMFLOAT3(x * -2, y * 2, z * 2);
				instance.cubeId = DirectX::XMFLOAT4(id, 0, 0, 0);
				instance.colorToApply = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); // Red
				instance.aabb.center = instance.Pos;
				instance.aabb.extents = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f); // Half the size of the cube

				instances.push_back(instance);
				id++;
				//this is for testing 
				//if (id % 2 != 0) {  // Check if ID is odd
				//	instance.colorToApply = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f); // Green
				//}
				//else {
				//	instance.colorToApply = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); // Red
				//}
				
			}
		}
	}
	


	vector<InstanceData>* const instancesPointer = &instances;



	GameObject Voxels(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1.0f, 1.0f, 1.0f), "Voxels", 0.0f, 0.0f);
	Voxels.AddShape(MeshType::CUBE, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1.0f, 1.0f, 1.0f), L"InstancedShader.fx", "Voxels", &instances);
	entities.push_back(Voxels);
	terrain = &Voxels;
}







void MainRender::SetEntityPointers()
{

	for (auto& entity : entities)
	{
		if (entity.Name() == "Terrain")
		{
			terrain = &entity;
		}
	
	}
}





MainRender::MainRender(const int& width, const int& height) :
	timescale(1.0f),
	width(width),
	height(height),
	isExit(false),
	isRocketLaunched(false),
	isEngineOn(false),
	inputManager(GetActiveWindow()) // Initialize the InputManager in the member initializer list with the active window handle

{
}


void MainRender::InitScene()
{
	if (!keyboard)
	{
		keyboard = make_unique<Keyboard>();
	}

	if (cameras.size() > 0)
	{
		cameras.clear();
	}
	if (entities.size() > 0)
	{
		entities.clear();
	}


	entities.reserve(8);

	// Function calls for Drawing 
	CreateCameras();
	CreateVoxels();
	
	SetEntityPointers();


}


void MainRender::ResetScene()
{
	isReset = true;
	
	timescale = 1;
	InitScene();
}




void MainRender::Update(const float& dt)
{

	CheckInput(dt);

	
	POINT cursorPos = GetCursorPosition();
	/*char buffer[100];
	sprintf_s(buffer, "Cursor Position: x = %d, y = %d\n", cursorPos.x, cursorPos.y);
	OutputDebugStringA(buffer);*/

	inputManager.Update();

	// Access mouse input data as needed
	if (inputManager.IsLeftButtonPressed())
	{
		OutputDebugStringA("Left mouse button pressed.\n");
	}

	if (inputManager.IsLeftButtonDown())
	{
		OutputDebugStringA("Left mouse button down.\n");
		// Mouse position in screen space (should be normalized [-1, 1])
		int mousePixelX = inputManager.GetMouseX();
		int mousePixelY = inputManager.GetMouseY();

		// Convert from pixel space to normalized space
		DirectX::XMFLOAT2 mousePos;
		mousePos.x = (2.0f * mousePixelX / width) - 1.0f;
		mousePos.y = 1.0f - (2.0f * mousePixelY / height);

		// Check if activeCam is not null
		if (activeCam)
		{
			// Camera's view and projection matrices
			DirectX::XMFLOAT4X4 viewMatrix = activeCam->View();
			DirectX::XMFLOAT4X4 projMatrix = activeCam->Projection();

			// Unproject mouse position to 3D world space
			DirectX::XMVECTOR mouseRayOrigin, mouseRayDir;

			mouseRayOrigin = DirectX::XMVector3Unproject(DirectX::XMVectorSet(mousePos.x, mousePos.y, 0.0f, 1.0f),
				0, 0, 1.0f, 1.0f,
				0, 1,
				DirectX::XMLoadFloat4x4(&projMatrix),
				DirectX::XMLoadFloat4x4(&viewMatrix),
				DirectX::XMMatrixIdentity());

			mouseRayDir = DirectX::XMVector3Unproject(DirectX::XMVectorSet(mousePos.x, mousePos.y, 1.0f, 1.0f),
				0, 0, 1.0f, 1.0f,
				0, 1,
				DirectX::XMLoadFloat4x4(&projMatrix),
				DirectX::XMLoadFloat4x4(&viewMatrix),
				DirectX::XMMatrixIdentity());

			// Normalize the ray direction
			mouseRayDir = DirectX::XMVector3Normalize(mouseRayDir - mouseRayOrigin);

			// You now have a ray that can be used

			// Loop through each GameObject (Voxel) in the entities list
			for (auto& voxel : entities)
			{
				// Get the instances (cubes) associated with this GameObject
				auto instances = voxel.GetInstances();

				// Loop through each instance (cube) and check for intersection
				for (auto& instance : instances)
				{
					if (AABBIntersect(instance.aabb, mouseRayOrigin, mouseRayDir))
					{
						char buffer[100];
						sprintf_s(buffer, "Raycast hit cube ID: %d\n", instance.id);
						OutputDebugStringA(buffer);

						break;
					}
				}
			}
		}
		else
		{
			// Handle error: activeCam is null
			OutputDebugStringA("Error: activeCam is null.\n");
		}

	}

	if (inputManager.IsLeftButtonReleased())
	{
		// Left mouse button was released
		OutputDebugStringA("Left mouse button released.\n");
	}

	// Get the mouse position from the InputManager
	int mouseX = inputManager.GetMouseX();
	int mouseY = inputManager.GetMouseY();


	
}

//



const Camera* const MainRender::ActiveCamera() const
{
	return activeCam;
}

const vector<GameObject>& MainRender::Entities() const
{
	return entities;
}

const bool& MainRender::ExitApplication() const
{
	return isExit;
}

